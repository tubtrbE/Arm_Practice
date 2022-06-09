/* USER CODE BEGIN WHILE */

while (1)
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD); // 시간 정보 얻어오기
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD); // 날짜 정보 얻어오기
	sprintf(temp, "\r\n20%02x-%02x-%02x %s %02x:%02x:%02x", sDate.Year, sDate.Month,
		sDate.Date, ampm[sTime.TimeFormat], sTime.Hours, sTime.Minutes,
		sTime.Seconds); // 형식에 맞게 출력

	FND(digit[3], Num(num[0]));
	FND(digit[2], Num(num[1]));
	FND(digit[1], Num(num[2]));
	FND(digit[0], Num(num[3]));

	if (start == 1) {
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

				sTime.Hours = (*(pS + 7) - '0') << 4;
				sTime.Hours += *(pS + 8) - '0';

				sTime.Minutes = (*(pS + 9) - '0') << 4;
				sTime.Minutes += *(pS + 10) - '0';

				sTime.Seconds = (*(pS + 11) - '0') << 4;
				sTime.Seconds += *(pS + 12) - '0';

				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			}
			else if (pD != 0) {

				sDate.Year = (*(pS + 7) - '0') << 4;
				sDate.Year += *(pS + 8) - '0';

				sDate.Month = (*(pS + 9) - '0') << 4;
				sDate.Month += *(pS + 10) - '0';

				sDate.Date = (*(pS + 11) - '0') << 4;
				sDate.Date += *(pS + 12) - '0';

				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
			}
		}
		if (pG != 0) {

			char* pT;
			char* pD;
			pT = strstr((char*)buf, "Time");
			pD = strstr((char*)buf, "Date");

			if (pT != 0) {

				// 시간 출력
				num[0] = (sTime.Hours & 0xf0) >> 4;
				num[1] = sTime.Hours & 0x0f;

				// 분 출력
				num[2] = (sTime.Minutes & 0xf0) >> 4;
				num[3] = sTime.Minutes & 0x0f;
			}
			else if (pD != 0) {

				// 월 출력
				num[0] = (sDate.Month & 0xf0) >> 4;
				num[1] = sDate.Month & 0x0f;

				// 일 출력
				num[2] = (sDate.Date & 0xf0) >> 4;
				num[3] = sDate.Date & 0x0f;
			}
			HAL_UART_Transmit(&huart3, (uint8_t*)temp, strlen(temp), 1000);
		}
		memset(buf, 0, sizeof(buf));
		start = 0;
	}

}
/* USER CODE END WHILE */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart->Instance == USART3) {
		//		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		if (rx == 3) {

			start = 1;
			buf[buf_index] = 0;
			// main 에서 buf_index 를 따로 사용하지 않으므로 buf_index 를 초기화 해주는 것이 좋다.
			// 만일 위에서 사용한다 하더라도 다른 변수를 선언해서 문제가 발생하지 않도록 해주어야 겠다.
			buf_index = 0;
		}
		else if (rx != 3) {
			buf[buf_index++] = rx;

		}

		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}