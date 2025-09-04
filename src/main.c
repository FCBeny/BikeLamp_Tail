#include "stm8s.h"

#define LED_GPIO_PORT    (GPIOC)
#define LED_L_GPIO_PIN   (GPIO_PIN_6)
#define LED_C_GPIO_PIN   (GPIO_PIN_5)
#define LED_R_GPIO_PIN   (GPIO_PIN_7)
#define BUTTON_GPIO_PORT (GPIOC)
#define BUTTON_GPIO_PIN  (GPIO_PIN_4)

typedef enum {
  LED_SLOW_BLINK_ALTER = 0,
  LED_SLOW_BLINK_ALTER_HALF,
  LED_SLOW_BLINK,
  LED_SLOW_BLINK_HALF,
  LED_CONST_ON,
  LED_CONST_ON_HALF,
  LED_FAST_BLINK,
  LED_FAST_BLINK_ALTER,
  LED_DOUBLE_BLINK,
  LED_WAVE,
  LED_POWER_SAVING,
  LED_MODE_COUNT
} LedMode_TypeDef;

void Delay(uint16_t nCount)
{
  while (nCount--)
  {
    __asm__("nop");
  }
}

void DelayMs(uint16_t ms)
{
  while (ms--)
  {
    Delay(250); // Approximate 1 ms delay
  }
}

void SetLED(uint8_t left, uint8_t center, uint8_t right)
{
  if (left)
  {
    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_L_GPIO_PIN);
  }
  else
  {
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_L_GPIO_PIN);
  }

  if (center)
  {
    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_C_GPIO_PIN);
  }
  else
  {
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_C_GPIO_PIN);
  }

  if (right)
  {
    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_R_GPIO_PIN);
  }
  else
  {
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_R_GPIO_PIN);
  }
}

void main(void)
{
  LedMode_TypeDef ledMode = LED_SLOW_BLINK;
  uint8_t prevButtonState = 1;
  uint8_t i = 0;

  GPIO_Init(LED_GPIO_PORT,    (GPIO_Pin_TypeDef)LED_L_GPIO_PIN,  GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_GPIO_PORT,    (GPIO_Pin_TypeDef)LED_C_GPIO_PIN,  GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_GPIO_PORT,    (GPIO_Pin_TypeDef)LED_R_GPIO_PIN,  GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(BUTTON_GPIO_PORT, (GPIO_Pin_TypeDef)BUTTON_GPIO_PIN, GPIO_MODE_IN_PU_IT);

  while (1)
  {
    uint8_t buttonState = GPIO_ReadInputPin(BUTTON_GPIO_PORT, (GPIO_Pin_TypeDef)BUTTON_GPIO_PIN);
    if (prevButtonState && !buttonState)
    {
      ledMode = (ledMode + 1) % LED_MODE_COUNT;
    }
    prevButtonState = buttonState;

    switch (ledMode)
    {
      case LED_CONST_ON:
        SetLED(1, 1, 1);
        break;

      case LED_CONST_ON_HALF:
        if (i <= 1)
        {
          SetLED(1, 1, 1);
        }
        else
        {
          SetLED(0, 0, 0);
          i = 0;
        }
        break;

      case LED_SLOW_BLINK:
        if (i <= 100)
        {
          SetLED(1, 1, 1);
        }
        else if (i <= 200)
        {
          SetLED(0, 0, 0);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_SLOW_BLINK_HALF:
        if (i <= 100)
        {
          SetLED(i % 2, i % 2, i % 2);
        }
        else if (i <= 200)
        {
          SetLED(0, 0, 0);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_SLOW_BLINK_ALTER:
        if (i <= 100)
        {
          SetLED(0, 1, 0);
        }
        else if (i <= 200)
        {
          SetLED(1, 0, 1);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_SLOW_BLINK_ALTER_HALF:
        if (i <= 100)
        {
          SetLED(0, i % 2, 0);
        }
        else if (i <= 200)
        {
          SetLED(i % 2, 0, i % 2);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_FAST_BLINK:
        if (i <= 30)
        {
          SetLED(1, 1, 1);
        }
        else if (i <= 60)
        {
          SetLED(0, 0, 0);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_FAST_BLINK_ALTER:
        if (i <= 30)
        {
          SetLED(0, 1, 0);
        }
        else if (i <= 60)
        {
          SetLED(1, 0, 1);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_DOUBLE_BLINK:
        if (i <= 15)
        {
          SetLED(1, 1, 1);
        }
        else if (i <= 30)
        {
          SetLED(0, 0, 0);
        }
        else if (i <= 45)
        {
          SetLED(1, 1, 1);
        }
        else if (i <= 180)
        {
          SetLED(0, 0, 0);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_WAVE:
        if (i <= 60)
        {
          SetLED(1, 0, 0);
        }
        else if (i <= 120)
        {
          SetLED(0, 1, 0);
        }
        else if (i <= 180)
        {
          SetLED(0, 0, 1);
        }
        else if (i <= 240)
        {
          SetLED(0, 1, 0);
        }
        else
        {
          i = 0;
        }
        break;

      case LED_POWER_SAVING:
        if (i <= 100)
        {
          SetLED(1, 0, 0);
        }
        else if (i <= 200)
        {
          SetLED(0, 0, 1);
        }
        else
        {
          i = 0;
        }
        break;

      default:
        break;
    }
    i++;
    DelayMs(5);
  }
}


/**
  C:\Users\xxx\.platformio\packages\tool-stm8flash>stm8flash.exe -u -c stlinkv2 -p STM8S103F3P6
  Determine OPT area
  Due to its file extension (or lack thereof), "Workaround" is considered as RAW BINARY format!
  Unlocked device. Option bytes reset to default state.
  Bytes written: 11
  */ 

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif