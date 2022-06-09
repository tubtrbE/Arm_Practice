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

// ��Ģ�� ����checksum ������ ��� ���ڸ� �����ִ� ����
uint8_t checksum = 0;
// rx �� ������ checksum �� ��ġ�ϴ��� �����ִ� ����
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
		// LED �κ�
		if (pL != 0) {

			for (uint8_t i = 0; i < 7; i++) {
				checksum += *(pL + i);
			}

			// checksum_rx �����
			// ������ rx �� bcd��Ģ(hexa)�� ���ؼ� ��Ʈ���� ���ش�
			checksum_rx = (*(pL + 7)) & 0xff;

			if ((checksum & 0xff) == checksum_rx) {
				//checksum
				printf("����!!!\n\r");
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
				printf("���ФФ�\n\r");
				printf("checksum : %x\r\n", checksum);
			}
		}

		// FND �κ�
		if (pF != 0) {

			// 5 == FND01(01234)  �����Ǿ��ִ� ������ ����
			// *(pF + 4) - '0' �� ���� �����Ǵ� �������� ���� �̹Ƿ�
			// ���� �����ָ� ���ϴ� ���̸� ���� �� �ְԵȴ�.
			uint8_t temp_length = 5 + (*(pF + 4) - '0');

			// ������ ���̸� �� �� �ְ� �Ǿ����Ƿ� �����Ǵ� ��ɾ� ���� ��ó ����������.
			for (uint8_t i = 0; i < temp_length; i++) {
				checksum += *(pF + i);
			}
			// printf�� �ڽ��� ��� ���� �¾Ҵ��� Ȯ���Ͽ���.
			printf("%d\r\n", temp_length);

			// checksum_rx �����
			// ������ rx �� bcd��Ģ(hexa)�� ���ؼ� ��Ʈ���� ���ش�
			checksum_rx = (*(pF + temp_length)) & 0xff;

			// ���� checksum �� checksum_rx�� ���Ͽ� �¾��� ��� 
			// FND�� ���� ���ָ� �ȴ�.
			// LED�� �̿� ���� ����̹Ƿ� �����ϵ��� �ϰڴ�.
			if ((checksum & 0xff) == checksum_rx) {
				//checksum
				printf("����!!!\n\r");
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
				printf("���ФФ�\n\r");
				printf("checksum : %x\r\n", checksum);
				printf("checksum_rx : %x\r\n", checksum_rx);
			}
		}
		// ���ͷ�Ʈ�� ��� ���� �ڿ� �������� ���� ����
		// �׸��� ���۵��� ��� �ʱ�ȭ ������� �����۵��Ѵ�.
		memset(buf, 0, sizeof(buf));
		checksum = 0;
		checksum_rx = 0;
		buf_index = 0;
		start = 0;
	}
}//while
  /* USER CODE END WHILE */

// call back ������ flag �� ��ɸ� ����ִ�.
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