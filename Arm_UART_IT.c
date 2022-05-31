void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	// uart 가 USART3의 데이터를 수신하였을때
	if (huart->Instance == USART3) {
		// 받았다는 사실을 그대로 전송해준다.
		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		// buf 의 buf_index 번째 에다가 rx 를 저장한다.
		buf[buf_index++] = rx;


		// 만약에 rx 에 \n 또는 \r 값을 입력하면
		if (rx == '\n' || rx == '\r') {
			// 줄이 끝났으니 null 문자를 추가해준다.
			buf[buf_index] = 0;
			// 문자열의 위치를 저장하기 위한 변수 p
			char* p = 0;
			//"led"의 위치를 반환하는 함수 strstr
			// 위치란 L = 시작점의 주소이다.
			p = strstr((char*)buf, "LED");

			// p가 0이 아니면 led 라는 글자가 있다는 뜻이다.
			if (p != 0) {

				// p + 3 은 led 의 pin 위치가 있다.
				// p + 4 은 led 의 pin 상태가 있다.
				// - '0' 을 함으로써 48 을 빼주어 정수값으로 처리하고자 한다.
				pin_num = *(p + 3) - '0';
				pin_state = *(p + 4) - '0';



				//led 는 0 ~ 7까지를 사용한다.
				if (0 <= pin_num && pin_num < 8) {
					// 만약에 핀 상태가 1이면 킨다.
					if (pin_state > 0) {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 1);
					}
					// 만약에 핀 상태가 0이면 킨다.
					else if (pin_state == 0) {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 0);
					}
				}
				// 나머지 a 의 경우에 모든것을 키는 if 문 실행
				else {
					if (pin_state > 0) {
						HAL_GPIO_WritePin(GPIOD, 0x00ff, 1);
					}
				}
			}
		}

		// 1bite의 단어를 rx 에다가 받는다, 맨아래 위치 하는 이유는 모든 callback 을 수행한뒤
		// 인터럽트를 수행하기 위해서 이다. (코드 진행 도중에 인터럽트가 발생하는것을 방지)

		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}