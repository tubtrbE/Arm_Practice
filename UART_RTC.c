
// RTC Date 구조체 내부 
typedef struct
{
	uint8_t WeekDay;  /*!< Specifies the RTC Date WeekDay.
						   This parameter can be a value of @ref RTC_WeekDay_Definitions */

	uint8_t Month;    /*!< Specifies the RTC Date Month (in BCD format).
						   This parameter can be a value of @ref RTC_Month_Date_Definitions */

	uint8_t Date;     /*!< Specifies the RTC Date.
						   This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

	uint8_t Year;     /*!< Specifies the RTC Date Year.
						   This parameter must be a number between Min_Data = 0 and Max_Data = 99 */

} RTC_DateTypeDef;


// RTC Time 구조체 내부 
typedef struct
{
	uint8_t Hours;            /*!< Specifies the RTC Time Hour.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected
								   This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */

	uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

	uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
								   This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

	uint8_t TimeFormat;       /*!< Specifies the RTC AM/PM Time.
								   This parameter can be a value of @ref RTC_AM_PM_Definitions */

	uint32_t SubSeconds;      /*!< Specifies the RTC_SSR RTC Sub Second register content.
								   This parameter corresponds to a time unit range between [0-1] Second
								   with [1 Sec / SecondFraction +1] granularity */

	uint32_t SecondFraction;  /*!< Specifies the range or granularity of Sub Second register content
								   corresponding to Synchronous prescaler factor value (PREDIV_S)
								   This parameter corresponds to a time unit range between [0-1] Second
								   with [1 Sec / SecondFraction +1] granularity.
								   This field will be used only by HAL_RTC_GetTime function */

	uint32_t DayLightSaving;  /*!< This interface is deprecated. To manage Daylight
								   Saving Time, please use HAL_RTC_DST_xxx functions */

	uint32_t StoreOperation;  /*!< This interface is deprecated. To manage Daylight
								   Saving Time, please use HAL_RTC_DST_xxx functions */
} RTC_TimeTypeDef;

/* USER CODE BEGIN PV */
uint8_t rx;
// 임시로 시간 및 날짜를 저장할 공간
char temp[100];
char ampm[2][3] = { "AM", "PM" };

// 아래 두개의 구조체를 선언해 주어야 
// 해당 변수를 사용 할 수 있다.
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
uint8_t buf[20] = { 0 };
uint8_t buf_index = 0;
/* USER CODE END PV */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{

	/*
	<<uart 명령어 예시>>
	SetTime134627\x03
	SetDate220608\x03
	GetTime\x03
	GetDate\x03
	*/

	if (huart->Instance == USART3) {

		if (rx == 3) {
			char* pS;
			char* pG;

			pS = strstr((char*)buf, "Set");
			pG = strstr((char*)buf, "Get");

			if (pS != 0) {

				char* pT;
				char* pD;
				pT = strstr((char*)buf, "Time");
				pD = strstr((char*)buf, "Date");

				if (pT != 0) {

					// BCD 법칙(헥사코드)에 따라 연산하므로 4bit 만큼 shift 해주어야 한다.
					sTime.Hours = (*(pS + 7) - '0') << 4;
					sTime.Hours += *(pS + 8) - '0';

					sTime.Minutes = (*(pS + 9) - '0') << 4;
					sTime.Minutes += *(pS + 10) - '0';

					sTime.Seconds = (*(pS + 11) - '0') << 4;
					sTime.Seconds += *(pS + 12) - '0';

					// 시
					printf("%c", *(pS + 7)); // 10
					printf("%c", *(pS + 8)); // 1

					// 분
					printf("%c", *(pS + 9)); // 10
					printf("%c", *(pS + 10)); // 1

					// 초
					printf("%c", *(pS + 11)); // 10
					printf("%c", *(pS + 12)); // 1
					printf("\n");
					HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
				}
				else if (pD != 0) {

					sDate.Year = (*(pS + 7) - '0') << 4;
					sDate.Year += *(pS + 8) - '0';

					sDate.Month = (*(pS + 9) - '0') << 4;
					sDate.Month += *(pS + 10) - '0';

					sDate.Date = (*(pS + 11) - '0') << 4;
					sDate.Date += *(pS + 12) - '0';

					// 시
					printf("%c", *(pS + 7)); // 10
					printf("%c", *(pS + 8)); // 1

					// 분
					printf("%c", *(pS + 9)); // 10
					printf("%c", *(pS + 10)); // 1

					// 초
					printf("%c", *(pS + 11)); // 10
					printf("%c", *(pS + 12)); // 1
					printf("\n");

					HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
				}
				memset(buf, 0, sizeof(buf));
				buf_index = 0;

			}
			else if (pG != 0) {

				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD); // 시간 정보 얻어오기
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD); // 날짜 정보 얻어오기


				char* pT;
				char* pD;
				pT = strstr((char*)buf, "Time");
				pD = strstr((char*)buf, "Date");

				if (pT != 0) {

					//temp 내부에 원하는 내용을 적어주는 함수 sprintf
					sprintf(temp, "\r\n%02x:%02x:%02x\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds);

				}
				else if (pD != 0) {
					sprintf(temp, "\r\n20%02x-%02x-%02x\r\n", sDate.Year, sDate.Month, sDate.Date);
				}
				HAL_UART_Transmit(&huart3, (uint8_t*)temp, strlen(temp), 1000);
			}
		}
		else if (rx != 3) {
			buf[buf_index++] = rx;
		}
		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}