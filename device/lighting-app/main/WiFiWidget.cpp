/*
 *
 *    Copyright (c) 2020-2023 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @file WiFiWidget.cpp
 *
 * Implements a WiFi Widget controller that display the state of bluetooth
 * connection on display.
 */

#include "WiFiWidget.h"

#include "esp_log.h"
#include "esp_system.h"
#if CONFIG_HAVE_DISPLAY
#include "ScreenManager.h"
#endif

static const char * TAG = "WiFiWidget";

void WiFiWidget::Init()
{
    ESP_LOGE(TAG, "@@@@@ START WiFiWidget::Init");
#if CONFIG_HAVE_DISPLAY
    mVLED = -1;
#endif // CONFIG_HAVE_DISPLAY

    mState = false;
    ESP_LOGE(TAG, "@@@@@ END WiFiWidget::Init");
}

void WiFiWidget::Set(bool state)
{
    ESP_LOGE(TAG, "@@@@@ START WiFiWidget::Set");
    bool stateChange = (mState != state);
    mState           = state;
    if (stateChange)
    {
#if CONFIG_HAVE_DISPLAY
        if (mVLED != -1)
        {
            ScreenManager::SetVLED(mVLED, mState);
        }
#endif // CONFIG_HAVE_DISPLAY
    }
    ESP_LOGE(TAG, "@@@@@ START WiFiWidget::Set");
}

#if CONFIG_HAVE_DISPLAY
void WiFiWidget::SetVLED(int id)
{
    ESP_LOGE(TAG, "@@@@@ START WiFiWidget::SetVLED");
    mVLED = id;
    if (mVLED != -1)
    {
        ScreenManager::SetVLED(mVLED, mState);
    }
    ESP_LOGE(TAG, "@@@@@ START WiFiWidget::SetVLED");
}
#endif // CONFIG_HAVE_DISPLAY
