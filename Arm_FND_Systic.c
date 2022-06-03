/* USER CODE BEGIN PV */
uint32_t cur_tick;
uint32_t start_tick;
uint32_t count = 9950;
uint8_t digit[4] = { 0 };
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
uint8_t Num(uint8_t num);
void FND(uint8_t com, uint8_t num);
/* USER CODE END PFP */

 /* USER CODE BEGIN WHILE */

  // HAL_GetTick() �Լ��� ���尡 ������ �������� �ð����� �������ش�.
start_tick = HAL_GetTick();
while (1)
{
	// ���� �ð����� cur_tick �� ������ ���������� ����� �ð� ���̸� �� �� �ְԵȴ�.
	cur_tick = HAL_GetTick();
	// �� �ð� ���̿� ���� ��� ���ָ� Ÿ�̸� ���̵� Ÿ�̸��� �ð� ����� �䳻 �� �� �ִ�.
	if ((cur_tick - start_tick) >= 250) {
		count++;
		if (count > 9999) {
			count = 0;
		}
		// ���������� ������ ���� �ð����� �ٽ� �ٲ��ָ� ����ؼ� �� �� �� �ְԵȴ�.
		start_tick = cur_tick;
	}

	// <�ڸ��� �˰���>
	// �� �ڸ����� ǥ���ϴ� ������� �Ϸ��� ��Ģ�� �ִ�.
	// 1�� �ڸ��� ���ϴ� (count%10) / 1 ����
	// �ڸ����� �����Ҷ����� 10�辿 �����ϴ� ��Ģ�� �ִ�.
	uint16_t a = 10;
	uint16_t b = 1;
	uint8_t mask = 0x10;

	for (int i = 0; i < 4; i++) {
		digit[i] = (count % a) / b;
		a *= 10;
		b *= 10;
	}

	// ���Ͱ��� �ڸ����� �����ϸ� for �� �����ε� �����ϰ� ó�� �� �� �ְԵȴ�.
	for (int i = 0; i < 4; i++) {
		FND(mask, Num(digit[i]));
		mask = mask << 1;
	}
}
/* USER CODE END WHILE */

/* USER CODE BEGIN 4 */
// ���ϴ� ���ڸ� ���� ��� �׿� �ش��ϴ� ����ڵ带
// �־��ִ� �Լ��̴�. 
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
	return 0x00;
}

// 8��Ʈ�� com �������� ������ num ������ �޾� 16��Ʈ�� 
// ����ڿ� ȭ�鿡 ����Ұ��̴�. 
void FND(uint8_t com, uint8_t num) {

	uint16_t data = 0;
	uint16_t mask = 0x0001;

	// �����ڸ��� �������ִ� com
	// ���׸�Ʈ�� �������ִ� num �� �־��ִ� �����̴�.
	data += com;
	data = data << 8;
	data += num;

	for (uint8_t i = 0; i < 16; i++) {
		// temp �� data ������ �� �ؾ� �ϱ� ������ 16bit�� ���� ���־�� �Ѵ�.
		uint16_t temp = 0;

		// temp �� 1���� Ŭ�� �� 0�϶��� ������ 16��Ʈ�� ���� ������ ���� �־��ش�.
		temp = data & mask;
		if (temp > 0) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 1);
		}
		else if (temp == 0) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 0);
		}
		// shift �� ���ִ� �κ��̴�.
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 0);
		mask = mask << 1;

	}

	// ���������� ��ġ �κп� �޽� ��ȣ�� �־��ָ� �����ص� �����Ͱ��� ���� �ȴ�.
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 1);

}