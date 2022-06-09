/* USER CODE BEGIN PV */
uint8_t rx, buf_index = 0;
uint8_t start = 0, stop = 0;
uint8_t stop_plus = 0;
uint8_t buf[20];
uint8_t buf_temp[20];
uint8_t d[4] = { 1,1,1,1 };
uint8_t digit[4] = { 0x10, 0x20, 0x40, 0x80 };
uint16_t digit_stop = 0;
uint8_t num[4] = { 10,10,10,10 };

// 규칙에 따라checksum 이전의 모든 문자를 더해주는 변수
uint8_t checksum = 0;
// rx 가 실제로 checksum 과 일치하는지 비교해주는 변수
uint8_t checksum_rx = 0;


/* USER CODE END PV */

  /* USER CODE BEGIN WHILE */

while (1)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, d[0]); // D1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, d[1]); // D2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, d[2]); // D3
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, d[3]);// D4

	for (int i = 0; i < 4; i++) {
		FND(digit[i], Num(num[i]));
	}

	if (start == 1) {

		char* pL;
		char* pF;
		pL = strstr((char*)buf, "LED");
		pF = strstr((char*)buf, "FND");
		// LED 부분
		if (pL != 0) {

			for (uint8_t i = 0; i < 7; i++) {
				checksum += *(pL + i);
			}

			// checksum_rx 만들기
			// 수신한 rx 를 bcd법칙(hexa)에 의해서 비트연산 해준다
			checksum_rx = (*(pL + 7)) & 0xff;

			if ((checksum & 0xff) == checksum_rx) {
				//checksum
				printf("성공!!!\n\r");
				printf("checksum : %x\r\n", checksum);
				if (*(pL + 3) - '0' == 0 && *(pL + 4) - '0' == 2) {

					for (int i = 0; i < 4; i++) {
						if (*(pL + 5) - '0' == 0 && *(pL + 6) - '0' == i + 1) {
							d[i] = 0;
						}

						else if (*(pL + 5) - '0' == 1 && *(pL + 6) - '0' == i + 1) {
							d[i] = 1;
						}
					}
				}
			}
			else {
				printf("실패ㅠㅠ\n\r");
				printf("checksum : %x\r\n", checksum);
			}
		}

		// FND 부분
		if (pF != 0) {

			// 5 == FND01(01234)  고정되어있는 데이터 길이
			// *(pF + 4) - '0' 의 값은 가변되는 데이터의 길이 이므로
			// 둘을 더해주면 원하는 길이를 얻을 수 있게된다.
			uint8_t temp_length = 5 + (*(pF + 4) - '0');

			// 데이터 길이를 알 수 있게 되었으므로 가변되는 명령어 에도 대처 가능해진다.
			for (uint8_t i = 0; i < temp_length; i++) {
				checksum += *(pF + i);
			}
			// printf로 자신의 결과 값이 맞았는지 확인하였다.
			printf("%d\r\n", temp_length);

			// checksum_rx 만들기
			// 수신한 rx 를 bcd법칙(hexa)에 의해서 비트연산 해준다
			checksum_rx = (*(pF + temp_length)) & 0xff;

			// 이제 checksum 과 checksum_rx을 비교하여 맞았을 경우 
			// FND를 제어 해주면 된다.
			// LED는 이와 같은 방식이므로 생략하도록 하겠다.
			if ((checksum & 0xff) == checksum_rx) {
				//checksum
				printf("성공!!!\n\r");
				printf("checksum : %x\r\n", checksum);
				printf("checksum_rx : %x\r\n", checksum_rx);

				//=========================================================
				if (*(pF + 3) - '0' == 0 && *(pF + 4) - '0' == 1) {
					num[0] = *(pF + 5) - '0';
					num[1] = 10;
					num[2] = 10;
					num[3] = 10;
				}
				//=========================================================
				if (*(pF + 3) - '0' == 0 && *(pF + 4) - '0' == 2) {
					for (int i = 0; i < 2; i++) {
						num[i] = *(pF + 5 + i) - '0';
					}
					num[2] = 10;
					num[3] = 10;
				}
				//=========================================================
				if (*(pF + 3) - '0' == 0 && *(pF + 4) - '0' == 3) {
					for (int i = 0; i < 3; i++) {
						num[i] = *(pF + 5 + i) - '0';
					}
					num[3] = 10;
				}
				//=========================================================
				if (*(pF + 3) - '0' == 0 && *(pF + 4) - '0' == 4) {
					for (int i = 0; i < 4; i++) {
						num[i] = *(pF + 5 + i) - '0';
					}
				}
				//=========================================================
			}
			else {
				printf("실패ㅠㅠ\n\r");
				printf("checksum : %x\r\n", checksum);
				printf("checksum_rx : %x\r\n", checksum_rx);
			}
		}
		// 인터럽트가 모두 끝난 뒤에 남아있을 만한 변수
		// 그리고 버퍼등을 모두 초기화 시켜줘야 정상작동한다.
		memset(buf, 0, sizeof(buf));
		checksum = 0;
		checksum_rx = 0;
		buf_index = 0;
		start = 0;
	}
}//while
  /* USER CODE END WHILE */

// call back 에서는 flag 의 기능만 들어있다.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART3) {

		//HAL_UART_Transmit(&huart3, &rx, 1, 100);

		if (rx == 3) {
			start = 1;
			buf[buf_index] = 0;
		}
		else if (rx != 3) {
			buf[buf_index++] = rx;
		}

		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}

}