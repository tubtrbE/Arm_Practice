/* USER CODE BEGIN PV */

// �ڸ����� ���� ����
uint8_t digit[4] = { 0 };
uint8_t num[4] = { 0 };
uint16_t a = 10;
uint16_t b = 1;
uint8_t mask_digit = 0x00;

// buf �� ���� �׸�, �� ��ġ�� �����ִ� ����
// �Ǿպ��� �����ؾ� �ϹǷ� 0���� �ʱ�ȭ ���ش�.
uint8_t buf[10], buf_index = 0;
// �׸��� ���� ����
uint8_t rx;

/* USER CODE END PV */

  /* USER CODE BEGIN WHILE */

while (1)
{
	// FND�� while loop �ȿ��� �⺻������ ����
	// ���Ѿ� IT �� ���� ��쿡 ���� �ϰԵȴ�.
	FND(digit[0], Num(num[0]));
	FND(digit[1], Num(num[1]));
	FND(digit[2], Num(num[2]));
	FND(digit[3], Num(num[3]));
}

/* USER CODE END WHILE */

// ���ϴ� ���ڸ� ���� ��� �׿� �ش��ϴ�
// ����ڵ带 �־��ش�.
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
	// turn off ���
	else if (num == 10) {
		return 0xff;
	}
	return 0xff;
}

// uart �ݹ� �Լ�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{

	// ���࿡ usart3 ä�ο��� ����� ���´ٸ�.
	if (huart->Instance == USART3) {

		// ����� Ȯ���� ���� ���� ���뿡 ���ؼ� �۽��� �ٽ� ���ش�.
		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		// ���๮�� ���� �ϳ��� �Է� �����ÿ� �����Ѵ�.
		if (rx == '\n' || rx == '\r') {

			// ���ڿ��� ���� �˸��� ���� \0 �� �߰����ش�.
			buf[buf_index] = 0;

			// null�� ������ buf ���θ� Ȯ���ϱ� ���ؼ�
			// for loop �� �̿��Ͽ� Ȯ���Ͽ���.
			for (uint8_t i = 0; i < 10; i++) {
				printf("%d", buf[i]);
			}
			printf("\r\n");

			// buf[2] �� 'on off' �� �����Ѵ�.====================================================
			if (0 <= buf[2] && buf[2] < 2) {
				if (buf[2] == 0) {
					buf[1] = 10;
				}
			}
			else {
				// 0 �� 1 �̿��� �Է��� ���� ��� ���Է��� ���� �ʱ�ȭ
				printf("\r\n �߸��� �Է��� �ϼ̽��ϴ�. (on off)\r\n");
			}

			// buf[0] �� '���ϴ� 1�ڸ�' or '����ڸ�'�� �����Ѵ�.====================================================
			// ���࿡ ��� �ڸ����� �ǹ��ϴ�'a' �Ǵ� 'A' �ΰ��
			if (buf[0] == 17 || buf[0] == 49) {
				digit[0] = 0x80;
				digit[1] = 0x40;
				digit[2] = 0x20;
				digit[3] = 0x10;
			}
			// ���ϴ� 1�ڸ��� ���� �ϴ� ���
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
				printf("\r\n �߸��� �Է��� �ϼ̽��ϴ�. (���ϴ� �ڸ�)\r\n");
			}

			// buf[1] �� '���ϴ� ����'�� �����Ѵ�.====================================================
			// 0 ~ 9 ������ ���ϴ� ����
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
				// 0 �� 1 �̿��� �Է��� ���� ��� ���Է��� ���� �ʱ�ȭ
				printf("\r\n �߸��� �Է��� �ϼ̽��ϴ�. (���ϴ� ����)\r\n");
			}
			buf_index = 0;
		}

		// ���๮�� �߻� ������ ��� ���۸� �����ϴ� ������ �ϴ� else if ��
		else if (buf_index < 11) {
			// ������ ���ؼ� ������ ���ڵ��� ��� '����' �̱� ������
			// �������·� ó���� ���ֱ� ���� ��ȯ�Ѵ�.
			buf[buf_index++] = rx - '0';
		}

		// call back ���ΰ� ��� ����ǰ� ���� IT �� �޴´�.
		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}