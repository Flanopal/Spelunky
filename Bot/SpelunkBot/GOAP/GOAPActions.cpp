#include "stdafx.h"
#include "GOAPActions.h"


// ------ GO TO DIRECTLY ------
GoToDirectly::GoToDirectly(FrameworkLibrary* lib, Coordinates target, int life, int rope)
{
	action = lib->playerActions->GoDirectlyToNode(target, life, rope);
}
GoToDirectly::GoToDirectly(FrameworkLibrary* lib, Coordinates target)
{
	action = lib->playerActions->GoDirectlyToNode(target);
}
bool GoToDirectly::WillFail()
{
	return !action->PlanPath();
}
void GoToDirectly::ExecuteAction()
{
	action->Start();
}
void GoToDirectly::StopAction()
{
	action->Stop();
}



// ------ EXPLORE TO TARGET ------
ExploreToTarget::ExploreToTarget(FrameworkLibrary* lib, Coordinates target, int life, int rope)
{
	action = lib->playerActions->ExploreToNode(target, life, rope);
}
ExploreToTarget::ExploreToTarget(FrameworkLibrary* lib, Coordinates target)
{
	action = lib->playerActions->ExploreToNode(target);
}
bool ExploreToTarget::WillFail()
{
	return !action->PlanPath();
}
void ExploreToTarget::ExecuteAction()
{
	action->Start();
}
void ExploreToTarget::StopAction()
{
	action->Stop();
}