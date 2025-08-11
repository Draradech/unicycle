#include "stdinc.h"
#include "esp32-hal-timer.h"
#include "esp_cpu.h"

static TaskHandle_t fastHandle;
static TaskHandle_t slowHandle;

void IRAM_ATTR fastISR()
{
  BaseType_t hasWoken = pdFALSE;
  vTaskNotifyGiveFromISR(fastHandle, &hasWoken);
  if (hasWoken) portYIELD_FROM_ISR();
}

uint8_t cnt;
void fastTask(void *p)
{
  for (;;)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    digitalWrite(4, HIGH);
    uint32_t t1 = esp_cpu_get_cycle_count();
    loop250us();
    cnt++;
    if (cnt == 8)
    {
      cnt = 0;
      if(slowHandle) xTaskNotifyGive(slowHandle);
    }
    uint32_t dtl = esp_cpu_get_cycle_count() - t1;
    uint16_t dt = dtl / 240;
    sensorData.taskTime[1][ttmin] = min(sensorData.taskTime[1][ttmin], dt);
    sensorData.taskTime[1][ttmax] = max(sensorData.taskTime[1][ttmax], dt);
    sensorData.taskTime[1][ttsum] += dt;
    sensorData.taskTime[1][ttnum]++;
    digitalWrite(4, LOW);
  }
}

void slowTask(void *p)
{
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    digitalWrite(2, HIGH);
    uint32_t t1 = esp_cpu_get_cycle_count();
    loop2ms();
    uint32_t dtl = esp_cpu_get_cycle_count() - t1;
    uint16_t dt = dtl / 240;
    sensorData.taskTime[0][ttmin] = min(sensorData.taskTime[0][ttmin], dt);
    sensorData.taskTime[0][ttmax] = max(sensorData.taskTime[0][ttmax], dt);
    sensorData.taskTime[0][ttsum] += dt;
    sensorData.taskTime[0][ttnum]++;
    digitalWrite(2, LOW);
  }
}

void setupSystem()
{
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);

  xTaskCreatePinnedToCore(fastTask, "fast", 4096, nullptr, 16, &fastHandle, 1);
  xTaskCreatePinnedToCore(slowTask, "slow", 4096, nullptr, 15, &slowHandle, 1);

  hw_timer_t *hwtim = timerBegin(/*freq=*/1000000);
  timerAttachInterrupt(hwtim, &fastISR);
  timerAlarm(hwtim, /*compare=*/250, /*autoReload=*/true, /*reloadCount=*/0);
}
