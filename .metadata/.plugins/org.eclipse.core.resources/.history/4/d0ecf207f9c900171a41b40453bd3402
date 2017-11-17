//These functions inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void MAX7219Send(unsigned char address, unsigned char data);
/**
* TODO: Show data on 7-seg via max7219_send
* Input:
* data: decimal value
* num_digs: number of digits will show on 7-seg
* Return:
* 0: success
* -1: illegal data range(out of 8 digits range)
*/
int display(int data, int num_digs)
{
	if (num_digs <= 8 && num_digs >= 0){
		int i;
		int div = 10000000;
		for (i = 8; i > num_digs; i--){
			MAX7219Send(i, 15);
			div = div / 10;
		}
		if (data < 0){
			MAX7219Send(num_digs, 10);
			data = -data;
			num_digs--;
			div = div / 10;
		}
		for (i = num_digs; i > 0; i--){
			int num = data / div;
			MAX7219Send(i, num);
			data = data % div;
			div = div / 10;
		}
		return 0;
	}else
		return -1;
}
void main()
{
	int student_id = 410001;
	GPIO_init();
	max7219_init();
	display(student_id, 7);
}
