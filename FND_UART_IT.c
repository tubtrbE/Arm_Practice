/* USER CODE BEGIN PV */

// 자릿수에 대한 변수
uint8_t digit[4] = { 0 };
uint8_t num[4] = { 0 };
uint16_t a = 10;
uint16_t b = 1;
uint8_t mask_digit = 0x00;

// buf 를 담을 그릇, 그 위치를 정해주는 변수
// 맨앞부터 시작해야 하므로 0으로 초기화 해준다.
uint8_t buf[10], buf_index = 0;
// 그릇에 담을 내용
uint8_t rx;

/* USER CODE END PV */

  /* USER CODE BEGIN WHILE */

while (1)
{
	// FND를 while loop 안에서 기본적으로 구동
	// 시켜야 IT 가 들어올 경우에 제어 하게된다.
	FND(digit[0], Num(num[0]));
	FND(digit[1], Num(num[1]));
	FND(digit[2], Num(num[2]));
	FND(digit[3], Num(num[3]));
}

/* USER CODE END WHILE */

// 원하는 숫자를 넣을 경우 그에 해당하는
// 헥사코드를 넣어준다.
uint8_t Num(uint8_t num) {

	if (num == 0) {
		return 0x03;
	}
	else if (num == 1) {
		return 0x9f;
	}
	else if (num == 2) {
		return 0x25;
	}
	else if (num == 3) {
		return 0x0d;
	}
	else if (num == 4) {
		return 0x99;
	}
	else if (num == 5) {
		return 0x49;
	}
	else if (num == 6) {
		return 0x41;
	}
	else if (num == 7) {
		return 0x1f;
	}
	else if (num == 8) {
		return 0x01;
	}
	else if (num == 9) {
		return 0x09;
	}
	// turn off 모드
	else if (num == 10) {
		return 0xff;
	}
	return 0xff;
}

// uart 콜백 함수
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{

	// 만약에 usart3 채널에서 통신이 들어온다면.
	if (huart->Instance == USART3) {

		// 통신이 확인을 위해 받은 내용에 대해서 송신을 다시 해준다.
		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		// 개행문자 둘중 하나를 입력 받을시에 동작한다.
		if (rx == '\n' || rx == '\r') {

			// 문자열의 끝을 알리기 위해 \0 을 추가해준다.
			buf[buf_index] = 0;

			// null을 포함한 buf 내부를 확인하기 위해서
			// for loop 를 이용하여 확인하였다.
			for (uint8_t i = 0; i < 10; i++) {
				printf("%d", buf[i]);
			}
			printf("\r\n");

			// buf[2] 는 'on off' 를 지정한다.====================================================
			if (0 <= buf[2] && buf[2] < 2) {
				if (buf[2] == 0) {
					buf[1] = 10;
				}
			}
			else {
				// 0 과 1 이외의 입력이 들어올 경우 재입력을 위해 초기화
				printf("\r\n 잘못된 입력을 하셨습니다. (on off)\r\n");
			}

			// buf[0] 은 '원하는 1자리' or '모든자리'를 지정한다.====================================================
			// 만약에 모든 자릿수를 의미하는'a' 또는 'A' 인경우
			if (buf[0] == 17 || buf[0] == 49) {
				digit[0] = 0x80;
				digit[1] = 0x40;
				digit[2] = 0x20;
				digit[3] = 0x10;
			}
			// 원하는 1자리를 제어 하는 경우
			else if (0 <= buf[0] && buf[0] < 4) {

				if (buf[0] == 0) {
					digit[0] = 0x80;
				}
				else if (buf[0] == 1) {
					digit[1] = 0x40;
				}
				else if (buf[0] == 2) {
					digit[2] = 0x20;
				}
				else if (buf[0] == 3) {
					digit[3] = 0x10;
				}

			}
			else {
				printf("\r\n 잘못된 입력을 하셨습니다. (원하는 자리)\r\n");
			}

			// buf[1] 는 '원하는 숫자'를 지정한다.====================================================
			// 0 ~ 9 사이의 원하는 숫자
			if (0 <= buf[1] && buf[1] <= 10) {
				if (buf[0] == 17 || buf[0] == 49) {
					for (uint8_t i = 0; i < 4; i++) {
						num[i] = buf[1];
					}
				}
				else {
					num[buf[0]] = buf[1];
				}
			}
			else {
				// 0 과 1 이외의 입력이 들어올 경우 재입력을 위해 초기화
				printf("\r\n 잘못된 입력을 하셨습니다. (원하는 숫자)\r\n");
			}
			buf_index = 0;
		}

		// 개행문자 발생 이전의 모든 버퍼를 저장하는 역할을 하는 else if 문
		else if (buf_index < 11) {
			// 수신을 통해서 들어오는 숫자들은 모두 '문자' 이기 때문에
			// 정수형태로 처리를 해주기 위해 변환한다.
			buf[buf_index++] = rx - '0';
		}

		// call back 내부가 모두 수행되고 난후 IT 를 받는다.
		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}