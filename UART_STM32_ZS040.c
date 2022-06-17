/* USER CODE BEGIN PV */
uint8_t rx2;
uint8_t rx3;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_UART_Receive_IT(&huart2, &rx2, 1);
HAL_UART_Receive_IT(&huart3, &rx3, 1);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{

	if (huart->Instance == USART3) {
		HAL_UART_Transmit(&huart2, &rx3, 1, 100);
		HAL_UART_Receive_IT(&huart3, &rx3, 1);
	}
	if (huart->Instance == USART2) {
		HAL_UART_Transmit(&huart3, &rx2, 1, 100);
		HAL_UART_Receive_IT(&huart2, &rx2, 1);
	}
}
/* USER CODE END 4 */
