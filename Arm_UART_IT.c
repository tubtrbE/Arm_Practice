





//==============================================================================
// ��� LED 8�� : PD0 ~ PD7
// ���� : TERA TERM �͹̳� �Է�
// ��� : UART SERIAL ��� => ���ͷ�Ʈ ���
//==============================================================================
// 
// 
//  
//==============================================================================
// /* USER CODE BEGIN PV */
// ���Ű��� �������ִ� rx����
uint8_t rx; 
// buf �� 50�ٱ��� ũ�� 50�� ¥�� , buf_index �� count �� ����
uint8_t buf[50], buf_index = 0; 
// �ɳѹ��� ���� ���ִ� ����
uint8_t pin_num;
// �� ���¸� ���� ���ִ� ����
uint8_t pin_state;
/* USER CODE END PV */
//==============================================================================
// �̸� �մܿ��� �ѹ� ���ͷ�Ʈ�� ���� ������ �������� �ʴ´�.
/* USER CODE BEGIN 2 */
HAL_UART_Receive_IT(&huart3, &rx, 1);
/* USER CODE END 2 */
//==============================================================================
/* USER CODE BEGIN 4 */
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
			char* p;
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
						HAL_GPIO_WritePin(GPIOD, 0x00ff, 0);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 1);
					}
					// ���࿡ �� ���°� 0�̸� ����.
					else if (pin_state == 0) {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << pin_num, 0);
					}
				}
				// ������ 'A' �� ��쿡 ������ Ű�� if ��, ���� else if �� ����
				else if (pin_num == 17) {
					if (pin_state > 0) {
						HAL_GPIO_WritePin(GPIOD, 0x00ff, 1);
					}
					else if (pin_state == 0) {
						HAL_GPIO_WritePin(GPIOD, 0x00ff, 0);
					}
				}
			}
			// �޸𸮴� �Է��� ó���� �Ŀ� ����ؼ� �ʱ�ȭ�� ����� ���� ��ɾ �Է� �� �� �ְԵȴ�.
			// buf_index �� ���� �ʱ�ȭ ���־�� buf�� 0���� �ٽ� ����Ѵ�.
			memset(buf, 0, sizeof(buf));
			buf_index = 0;
		}
		// 1bite�� �ܾ rx ���ٰ� �޴´�, �ǾƷ� ��ġ �ϴ� ������ ��� callback �� �����ѵ�
		// ���ͷ�Ʈ�� �����ϱ� ���ؼ� �̴�. (�ڵ� ���� ���߿� ���ͷ�Ʈ�� �߻��ϴ°��� ����)
		HAL_UART_Receive_IT(&huart3, &rx, 1);
	}
}
/* USER CODE END 4 */
