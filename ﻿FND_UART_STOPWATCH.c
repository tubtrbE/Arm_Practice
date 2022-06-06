/* USER CODE BEGIN PV */

	// mcu ���� �ð��� ���ϱ� ���� ���� �ð�
uint32_t start_tick, cur_tick;
// ī��Ʈ ���� (���� ������ �ð� ex) 1)
uint32_t count = 0;
// �ڸ����� ���� ����
uint8_t digit[4] = { 0x10, 0x20, 0x40, 0x80 };
uint16_t digit_stop = 0;
uint8_t num[4] = { 0 };

// buf �� ���� �׸�, �� ��ġ�� �����ִ� ����
// �Ǿպ��� �����ؾ� �ϹǷ� 0���� �ʱ�ȭ ���ش�.
uint8_t buf[10], buf_index = 0;
// �׸��� ���� ����
uint8_t rx;

/* USER CODE END PV */

  /* USER CODE BEGIN WHILE */

start_tick = HAL_GetTick();
while (1)
{
	cur_tick = HAL_GetTick();
	if ((cur_tick - start_tick) >= 250) {

		// ���࿡ digit_stop �� ���� ������ �ʾ�����
		// default �� 0
		if (digit_stop == 0) {
			count = 0;
		}
		// ���Ŀ� IT �󿡼� digit_stop ���� �ԷµǸ� count++ ����
		// 0s �Է½ÿ� 0���� �ʱ�ȭ
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

// uart �ݹ� �Լ�
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
			// count �� ���ϴ� digit_stop �� ������ش�.
			// �ð��� ������ min �ϰ��
			if ((p = strstr((char*)buf, "M")) != 0 || (p = strstr((char*)buf, "m")) != 0) {
				// ���ο� �Է��� ��� �����Ƿ� digit_stop�� �ʱ�ȭ ���ش�.
				digit_stop = 0;
				uint8_t temp = 0;
				uint16_t temp_digit = 1;

				for (int i = 1; i < buf_index; i++) {
					digit_stop = digit_stop + temp_digit * (*(p - i) - '0');
					temp_digit *= 10;
				}
				// ������ min �̹Ƿ� 60�� ���Ͽ� �ش�.
				digit_stop = digit_stop * 60;
				if (digit_stop > 9999) {
					digit_stop = 9999;
					printf("�ִ밪�� 9999�� ����\r\n");
				}
				printf("digit_stop(�����ð�) : %d\r\n", digit_stop);
			}
			// �ð��� ������ sec �ϰ��
			else if ((p = strstr((char*)buf, "S")) != 0 || (p = strstr((char*)buf, "s")) != 0) {
				// ���ο� �Է��� ��� �����Ƿ� digit_stop�� �ʱ�ȭ ���ش�.
				digit_stop = 0;
				uint8_t temp = 0;
				uint16_t temp_digit = 1;
				for (int i = 1; i < buf_index; i++) {
					digit_stop = digit_stop + temp_digit * (*(p - i) - '0');
					temp_digit *= 10;
					printf("digit_stop(�����ð�) : %d\r\n", digit_stop);
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
