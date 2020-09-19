#include "routes/get.h"
#include "routes/change_interval.h"
#include "routes/reset.h"
#include "routes/index.h"
#include "routes/change_wifi.h"
#include "routes/syncTime.h"
#include "routes/water_this_day.h"

void initRoutes() {
  routeIndex();
  routeChangeInterval();
  routeChangeWifi();
  routeGet();
  routeReset();
  routeSyncTime();
  waterThisDay();
}
