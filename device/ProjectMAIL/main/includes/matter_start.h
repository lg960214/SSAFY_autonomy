#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_matter.h>
#include <esp_matter_console.h>

#include <app_priv.h>
#include <app_reset.h>
#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>


extern "C" void matter_start();
