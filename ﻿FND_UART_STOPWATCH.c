/* USER CODE BEGIN PV */

	// mcu 시작 시간과 비교하기 위한 현재 시간
uint32_t start_tick, cur_tick;
// 카운트 변수 (내가 설정한 시간 ex) 1)
uint32_t count = 0;
// 자릿수에 대한 변수
uint8_t digit[4] = { 0x10, 0x20, 0x40, 0x80 };
uint16_t digit_stop = 0;
uint8_t num[4] = { 0 };

// buf 를 담을 그릇, 그 위치를 정해주는 변수
// 맨앞부터 시작해야 하므로 0으로 초기화 해준다.
uint8_t buf[10], buf_index = 0;
// 그릇에 담을 내용
uint8_t rx;

/* USER CODE END PV */

  /* USER CODE BEGIN WHILE */

start_tick = HAL_GetTick();
while (1)
{
	cur_tick = HAL_GetTick();
	if ((cur_tick - start_tick) >= 250) {

		// 만약에 digit_stop 이 따로 들어오지 않았을때
		// default 값 0
		if (digit_stop == 0) {
			count = 0;
		}
		// 이후에 IT 상에서 digit_stop 값이 입력되면 count++ 시작
		// 0s 입력시에 0으로 초기화
		else if (digit_stop > 0 && count < digit_stop) {
			count++;
		}
		if (count > 9999) {
			count = 0;
		}
		start_tick = cur_tick;
		uint16_t a = 10;
		uint16_t b = 1;
		for (int i = 0; i < 4; i++) {
			num[i] = (count % a) / b;
			a *= 10;
			b *= 10;
		}
	}

	for (int i = 0; i < 4; i++) {
		FND(digit[i], Num(num[i]));
	}
}

/* USER CODE END WHILE */

// uart 콜백 함수
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{

	if (huart->Instance == USART3) {

		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		if (rx == '\n' || rx == '\r') {

			buf[buf_index] = 0;
			for (uint8_t i = 0; i < 10; i++) {
				printf("%d", buf[i]);
			}
			printf("\r\n");

			char* p;
			// count 와 비교하는 digit_stop 을 만들어준다.
			// 시간의 단위가 min 일경우
			if ((p = strstr((char*)buf, "M")) != 0 || (p = strstr((char*)buf, "m")) != 0) {
				// 새로운 입력이 들어 왔으므로 digit_stop을 초기화 해준다.
				digit_stop = 0;
				uint8_t temp = 0;
				uint16_t temp_digit = 1;

				for (int i = 1; i < buf_index; i++) {
					digit_stop = digit_stop + temp_digit * (*(p - i) - '0');
					temp_digit *= 10;
				}
				// 단위가 min 이므로 60을 곱하여 준다.
				digit_stop = digit_stop * 60;
				if (digit_stop > 9999) {
					digit_stop = 9999;
					printf("최대값인 9999로 설정\r\n");
				}
				printf("digit_stop(정지시간) : %d\r\n", digit_stop);
			}
			// 시간의 단위가 sec 일경우
			else if ((p = strstr((char*)buf, "S")) != 0 || (p = strstr((char*)buf, "s")) != 0) {
				// 새로운 입력이 들어 왔으므로 digit_stop을 초기화 해준다.
				digit_stop = 0;
				uint8_t temp = 0;
				uint16_t temp_digit = 1;
				for (int i = 1; i < buf_index; i++) {
					digit_stop = digit_stop + temp_digit * (*(p - i) - '0');
					temp_digit *= 10;
					printf("digit_stop(정지시간) : %d\r\n", digit_stop);
				}
			}

			memset(buf, 0, sizeof(buf));
			buf_index = 0;
		}
		else if (buf_index < 11) {
			buf[buf_index++] = rx;
		}
		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}
