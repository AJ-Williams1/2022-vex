#include "main.h"
#include "ports.h"

using namespace pros;

void index_disc()
{
    ADIDigitalOut indexer(INDEX_PORT);
    Motor intake(INTAKE_PORT);

    intake.move(127);
    indexer.set_value(false); // Fire
    delay(250);
    indexer.set_value(true); // Retract indexer
    delay(500); // Let intake run a little longer to make sure next disc falls down
    intake.brake();
}
