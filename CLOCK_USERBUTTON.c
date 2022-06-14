// 모드를 설정하는 유저 버튼의 중요 부분만을 편집한 것입니다.

// 첫번째 while loop 는 main loop 이고 두번째 while loop 가 
// 인터럽트 발생시 작동하는 while loop 입니다.
while (1) {
	//mode 를 정해주는 while loop
	while (rising_edge >= 1) {
		cur_tick = HAL_GetTick();
		tick_gap = cur_tick - start_tick;

		// 더블 클릭을 구분 하는 시간을 300ms 로 만들어 주었 습니다.
		if (tick_gap >= 300) {

			// one click 을 구분 하는 알고리즘 입니다.
			if (rising_edge == 1 && falling_edge >= 1) {
				rising_edge = 0;
				falling_edge = 0;
				printf("one click==========================\r\n");
			}
			// double click 을 구분 하는 알고리즘 입니다.
			if (rising_edge >= 2 && falling_edge >= 1) {
				rising_edge = 0;
				falling_edge = 0;
				printf("two click++++++++++++++++++++++++++\r\n");
			}
			// long click 을 구분 하는 알고리즘 입니다.
			if (tick_gap >= 2000 && falling_edge == 0) {
				rising_edge = 0;
				falling_edge = 0;
				printf("long click//////////////////////////\r\n");
			}
		}
		// delay 를 넣지 않으면 너무 빠르게 인식 하여 넣어주었습니다.
		HAL_Delay(100);
	}
}


// 인터럽트 call back 부분이다.
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

