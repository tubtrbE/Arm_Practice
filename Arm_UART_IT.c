void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	// uart �� USART3�� �����͸� �����Ͽ�����
	if (huart->Instance == USART3) {
		// �޾Ҵٴ� ����� �״�� �������ش�.
		HAL_UART_Transmit(&huart3, &rx, 1, 100);

		// buf �� buf_index ��° ���ٰ� rx �� �����Ѵ�.
		buf[buf_index++] = rx;


		// ���࿡ rx �� \n �Ǵ� \r ���� �Է��ϸ�
		if (rx == '\n' || rx == '\r') {
			// ���� �������� null ���ڸ� �߰����ش�.
			buf[buf_index] = 0;
			// ���ڿ��� ��ġ�� �����ϱ� ���� ���� p
			char* p = 0;
			//"led"�� ��ġ�� ��ȯ�ϴ� �Լ� strstr
			// ��ġ�� L = �������� �ּ��̴�.
			p = strstr((char*)buf, "LED");

			// p�� 0�� �ƴϸ� led ��� ���ڰ� �ִٴ� ���̴�.
			if (p != 0) {

				// p + 3 �� led �� pin ��ġ�� �ִ�.
				// p + 4 �� led �� pin ���°� �ִ�.
				// - '0' �� �����ν� 48 �� ���־� ���������� ó���ϰ��� �Ѵ�.
				pin_num = *(p + 3) - '0';
				pin_state = *(p + 4) - '0';



				//led �� 0 ~ 7������ ����Ѵ�.
				if (0 <= pin_num && pin_num < 8) {
					// ���࿡ �� ���°� 1�̸� Ų��.
					if (pin_state > 0) {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 1);
					}
					// ���࿡ �� ���°� 0�̸� Ų��.
					else if (pin_state == 0) {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 0);
					}
				}
				// ������ a �� ��쿡 ������ Ű�� if �� ����
				else {
					if (pin_state > 0) {
						HAL_GPIO_WritePin(GPIOD, 0x00ff, 1);
					}
				}
			}
		}

		// 1bite�� �ܾ rx ���ٰ� �޴´�, �ǾƷ� ��ġ �ϴ� ������ ��� callback �� �����ѵ�
		// ���ͷ�Ʈ�� �����ϱ� ���ؼ� �̴�. (�ڵ� ���� ���߿� ���ͷ�Ʈ�� �߻��ϴ°��� ����)

		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}