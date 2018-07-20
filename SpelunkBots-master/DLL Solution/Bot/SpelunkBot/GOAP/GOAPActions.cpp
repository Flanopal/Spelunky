#include "stdafx.h"
#include "GOAPActions.h"


unique_ptr<IGOAPGoal> IGOAPGoal::GetGoal(GOAPActionID ID, FrameworkLibrary* lib, Coordinates target, int life, int rope)
{
	switch (ID)
	{
	case GOAPActionID::GoToDirectly: return make_unique<Goals::GoToDirectly>(lib, target, life, rope);
	case GOAPActionID::ExploreTo: return make_unique<Goals::ExploreToTarget>(lib, target, life, rope);
	case GOAPActionID::GetRope: return make_unique<Goals::GetRope>(lib);
	case GOAPActionID::GoToNode: return make_unique<Goals::GoToNode>(lib, target, life, rope);
	case GOAPActionID::ExploreCave: return make_unique<Goals::ExploreCave>(lib, life, rope);
	default:return nullptr;
	}
}
unique_ptr<IGOAPGoal> IGOAPGoal::GetGoal(GOAPActionID ID, FrameworkLibrary* lib, Coordinates target)
{
	switch (ID)
	{
	case GOAPActionID::GoToDirectly: return make_unique<Goals::GoToDirectly>(lib, target);
	case GOAPActionID::ExploreTo: return make_unique<Goals::ExploreToTarget>(lib, target);
	case GOAPActionID::GetRope: return make_unique<Goals::GetRope>(lib);
	case GOAPActionID::GoToNode: return make_unique<Goals::GoToNode>(lib, target);
	case GOAPActionID::ExploreCave: return make_unique<Goals::ExploreCave>(lib);
	default:return nullptr;
	}
}

// ------ GO TO DIRECTLY ------
Goals::GoToDirectly::GoToDirectly(FrameworkLibrary* lib, Coordinates target, int life, int rope):life(life),rope(rope)
{
	action = lib->playerActions->GoDirectlyToNode(target, life, rope);
}
Goals::GoToDirectly::GoToDirectly(FrameworkLibrary* lib, Coordinates target)
{
	action = lib->playerActions->GoDirectlyToNode(target);
}
bool Goals::GoToDirectly::WillFail(vector<GOAPActionID> const providedGoals)
{
	if (action->PlanPath())
	{
		life = action->FinalLifeCount();
		rope = action->FinalRopeCount();
		return false;
	}
	return true;
}
void Goals::GoToDirectly::ExecuteAction()
{
	action->Start();
}
void Goals::GoToDirectly::StopAction()
{
	action->Stop();
}



// ------ EXPLORE TO TARGET ------
Goals::ExploreToTarget::ExploreToTarget(FrameworkLibrary* lib, Coordinates target, int life, int rope)
{
	action = lib->playerActions->ExploreToNode(target, life, rope);
}
Goals::ExploreToTarget::ExploreToTarget(FrameworkLibrary* lib, Coordinates target)
{
	action = lib->playerActions->ExploreToNode(target);
}
bool Goals::ExploreToTarget::WillFail(vector<GOAPActionID> const providedGoals)
{
	if (action->PlanPath())
	{
		life = action->FinalLifeCount();
		rope = action->FinalRopeCount();
		return false;
	}
	return true;
}
void Goals::ExploreToTarget::ExecuteAction()
{
	action->Start();
}
void Goals::ExploreToTarget::StopAction()
{
	action->Stop();
}


// ------ GO TO NODE ------
Goals::GoToNode::GoToNode(FrameworkLibrary* lib, Coordinates target, int life, int rope):lib(lib),target(target),life(life),rope(rope)
{ }
Goals::GoToNode::GoToNode(FrameworkLibrary* lib, Coordinates target) : lib(lib), target(target)
{ }
bool Goals::GoToNode::WillFail(vector<GOAPActionID> const providedGoals)
{
	if (!transformed)
	{
		transform = TransformIntoSubGoals(providedGoals);
		transformed = true;
	}
	if (transform.size() == 0) return true;
	return false;
}
bool Goals::GoToNode::Preserve()
{
	return preserve;
}
vector<unique_ptr<IGOAPGoal>> Goals::GoToNode::TransformIntoSubGoals(vector<GOAPActionID> const providedGoals)
{
	if (transformed) // already tranformed -> return vector
	{
		transformed = false;
		return move(transform);
	}
	vector<unique_ptr<IGOAPGoal>> ret;
	for (size_t i = 0; i < subGoals.size(); ++i)
	{
		int opLife = life;
		int opRope = rope;
		preserve = false;
		vector<GOAPActionID> subsequence = subGoals[i];
		bool condition = true;
		// control if desired action are supported by GOAP
		for (size_t j = 0; j < subsequence.size(); ++j)
		{
			if (subsequence[j] == GOAPActionID::Preserve) continue;
			if (find(providedGoals.begin(), providedGoals.end(), subsequence[j]) == providedGoals.end())
			{
				condition = false;
				break;
			}
		}
		if (!condition) continue;
		condition = true;
		// control if desired actions will not fail
		for (size_t j = 0; j < subsequence.size(); ++j)
		{
			if (subsequence[j] == GOAPActionID::Preserve) { preserve = true; continue; }
			unique_ptr<IGOAPGoal> Goal;
			if (life != -1) Goal = GetGoal(subsequence[j], lib, target, opLife, opRope);
			else Goal = GetGoal(subsequence[j], lib, target);
			if (!Goal->WillFail(providedGoals))
			{
				opLife = Goal->GetLifes();
				opRope = Goal->GetRopes();
				ret.push_back(move(Goal));				
			}
			else
			{
				// subgaol failed -> other subgoal sequention will be tried
				ret.clear();
				condition = false;
				break;
			}
		}
		if (condition)
		{
			// succesfully tranformed -> save final rope and life count
			life = opLife;
			rope = opRope;
			break;
		}
	}
	return ret;
}

// ------ GET ROPE ------
Goals::GetRope::GetRope(FrameworkLibrary* lib) :lib(lib) {}
bool Goals::GetRope::WillFail(vector<GOAPActionID> const providedGoals)
{
	if (!actionAssigned) AssignAction();
	return action == nullptr;
}
void Goals::GetRope::ExecuteAction()
{
	if (!actionAssigned) AssignAction();
	if (action == nullptr) return;
	action->Start();
}
void Goals::GetRope::StopAction()
{
	if (!actionAssigned) AssignAction();
	if (action == nullptr) return;
	action->Stop();
}
ActionState Goals::GetRope::GetState()
{ 
	if (action != nullptr) 
		return action->GetState(); 
	return ActionState::waiting;
}
void Goals::GetRope::AssignAction()
{
	if (actionAssigned) return;
	actionAssigned = true;
	vector<Coordinates> objects = lib->mapControl->GetRopesAndCrates();
	for (size_t i = 0; i < objects.size(); ++i)
	{
		auto there = lib->playerActions->GoDirectlyToNode(objects[i],1,0);
		if (!there->PlanPath()) continue; // cant get there
		Coordinates pos = lib->GetPrevPossition();
		lib->pathSearch->SetLifesForSearch(0);
		lib->pathSearch->SetRopesForSearch(0);
		auto back = lib->pathSearch->FindAndGetPath(objects[i], pos);
		Coordinates finish = back[0]->GetFinalCoords();
		if ((int)pos.x != (int)finish.x || (int)pos.x != (int)finish.x) continue; // there is no way back
		// there is go there and back again actions
		auto list = lib->playerActions->movements->GetActionList();
		list->AddAction(move(there));
		list->AddAction(lib->playerActions->PickRopeOrOpenCrate());
		list->AddAction(lib->playerActions->ExecutePath(move(back)));
		action = move(list);
		return;
	}	
}

// ------ EXPLORE CAVE ------ 
Goals::ExploreCave::ExploreCave(FrameworkLibrary* lib, int life, int rope):lib(lib),life(life),rope(rope) {}
Goals::ExploreCave::ExploreCave(FrameworkLibrary* lib):lib(lib) {}
bool Goals::ExploreCave::WillFail(vector<GOAPActionID> const providedGoals)
{
	if (goToCorner == nullptr) SetGoToCorner(providedGoals);
	return goToCorner->WillFail(providedGoals);
}
vector<unique_ptr<IGOAPGoal>> Goals::ExploreCave::TransformIntoSubGoals(vector<GOAPActionID> const providedGoals)
{
	if (goToCorner == nullptr) SetGoToCorner(providedGoals);
	corners[GetClosestCorner()].x = -1; // processed corner
	vector<unique_ptr<IGOAPGoal>> ret;
	ret.push_back(move(goToCorner));
	return ret;
}
bool Goals::ExploreCave::Preserve()
{
	return GetClosestCorner() != -1;
}
void Goals::ExploreCave::SetGoToCorner(vector<GOAPActionID> const providedGoals)
{
	if (find(providedGoals.begin(), providedGoals.end(), GOAPActionID::GoToNode) == providedGoals.end())
		return;
	while (goToCorner == nullptr)
	{
		int index = GetClosestCorner();
		if (index == -1) return;
		if (life != -1)
		{
			goToCorner = GetGoal(GOAPActionID::GoToNode, lib, corners[index], life, rope);
		}
		else
		{
			goToCorner = GetGoal(GOAPActionID::GoToNode, lib, corners[index]);
		}
		if (goToCorner->WillFail(providedGoals))
		{
			goToCorner = nullptr;
			corners[index].x = -1;
			continue;
		}
		if (life != -1)
		{
			life = goToCorner->GetLifes();
			rope = goToCorner->GetRopes();
		}
	}
}
int Goals::ExploreCave::GetClosestCorner()
{
	Coordinates pos = lib->GetPrevPossition();
	double dist = 500;
	int index = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (corners[i].x < 0) continue; // already visited
		double newDist = abs(corners[i].x - pos.x) + abs(corners[i].y - pos.y);
		if (newDist < dist)
		{
			index = i;
			dist = newDist;
		}
	}
	return index;
}
