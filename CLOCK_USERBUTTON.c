// ��带 �����ϴ� ���� ��ư�� �߿� �κи��� ������ ���Դϴ�.

// ù��° while loop �� main loop �̰� �ι�° while loop �� 
// ���ͷ�Ʈ �߻��� �۵��ϴ� while loop �Դϴ�.
while (1) {
	//mode �� �����ִ� while loop
	while (rising_edge >= 1) {
		cur_tick = HAL_GetTick();
		tick_gap = cur_tick - start_tick;

		// ���� Ŭ���� ���� �ϴ� �ð��� 300ms �� ����� �־� ���ϴ�.
		if (tick_gap >= 300) {

			// one click �� ���� �ϴ� �˰��� �Դϴ�.
			if (rising_edge == 1 && falling_edge >= 1) {
				rising_edge = 0;
				falling_edge = 0;
				printf("one click==========================\r\n");
			}
			// double click �� ���� �ϴ� �˰��� �Դϴ�.
			if (rising_edge >= 2 && falling_edge >= 1) {
				rising_edge = 0;
				falling_edge = 0;
				printf("two click++++++++++++++++++++++++++\r\n");
			}
			// long click �� ���� �ϴ� �˰��� �Դϴ�.
			if (tick_gap >= 2000 && falling_edge == 0) {
				rising_edge = 0;
				falling_edge = 0;
				printf("long click//////////////////////////\r\n");
			}
		}
		// delay �� ���� ������ �ʹ� ������ �ν� �Ͽ� �־��־����ϴ�.
		HAL_Delay(100);
	}
}


// ���ͷ�Ʈ call back �κ��̴�.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	// rising edge
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
		rising_edge++;
		printf("rising edge : %d\r\n", rising_edge);
		start_tick = HAL_GetTick();
	}

	// falling edge
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) {
		falling_edge++;
		printf("falling edge : %d\r\n", falling_edge);
	}
}

