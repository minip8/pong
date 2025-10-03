#include "controller.h"
#include "ai.h"

AIController::AIController(AI& ai) :
    m_AI{ai}
{ assert(m_AI.getRunningStatus()); }

Key AIController::getMove() {
    return m_AI.getMove();
}