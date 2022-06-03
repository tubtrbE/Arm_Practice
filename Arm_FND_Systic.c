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

  // HAL_GetTick() 함수는 보드가 시작한 지점부터 시간값을 리턴해준다.
start_tick = HAL_GetTick();
while (1)
{
	// 현재 시간값인 cur_tick 을 받으면 시작점과의 상대적 시간 차이를 알 수 있게된다.
	cur_tick = HAL_GetTick();
	// 그 시간 차이에 따라 제어를 해주면 타이머 없이도 타이머의 시간 기능을 흉내 낼 수 있다.
	if ((cur_tick - start_tick) >= 250) {
		count++;
		if (count > 9999) {
			count = 0;
		}
		// 마지막으로 비교점을 현재 시간으로 다시 바꿔주면 계속해서 비교 할 수 있게된다.
		start_tick = cur_tick;
	}

	// <자릿수 알고리즘>
	// 각 자릿수를 표현하는 방법에는 일련의 규칙이 있다.
	// 1의 자리를 비교하는 (count%10) / 1 에서
	// 자릿수가 증가할때마다 10배씩 증가하는 규칙이 있다.
	uint16_t a = 10;
	uint16_t b = 1;
	uint8_t mask = 0x10;

	for (int i = 0; i < 4; i++) {
		digit[i] = (count % a) / b;
		a *= 10;
		b *= 10;
	}

	// 위와같이 자릿수를 저장하면 for 문 만으로도 간단하게 처리 할 수 있게된다.
	for (int i = 0; i < 4; i++) {
		FND(mask, Num(digit[i]));
		mask = mask << 1;
	}
}
/* USER CODE END WHILE */

/* USER CODE BEGIN 4 */
// 원하는 숫자를 넣을 경우 그에 해당하는 헥사코드를
// 넣어주는 함수이다. 
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

// 8비트의 com 레지스터 정보와 num 정보를 받아 16비트를 
// 만든뒤에 화면에 출력할것이다. 
void FND(uint8_t com, uint8_t num) {

	uint16_t data = 0;
	uint16_t mask = 0x0001;

	// 점멸자리를 지정해주는 com
	// 세그먼트를 지정해주는 num 을 넣어주는 과정이다.
	data += com;
	data = data << 8;
	data += num;

	for (uint8_t i = 0; i < 16; i++) {
		// temp 가 data 변수와 비교 해야 하기 때문에 16bit를 지정 해주어야 한다.
		uint16_t temp = 0;

		// temp 가 1보다 클때 와 0일때로 나누어 16비트로 만든 데이터 값을 넣어준다.
		temp = data & mask;
		if (temp > 0) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 1);
		}
		else if (temp == 0) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 0);
		}
		// shift 를 해주는 부분이다.
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 0);
		mask = mask << 1;

	}

	// 마지막으로 래치 부분에 펄스 신호를 넣어주면 저장해둔 데이터값이 들어가게 된다.
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 0);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 1);

}