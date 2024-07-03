
把PC端IP设置为192.168.1.XXX 

XXX为除100外的 0-255内的数字

连接网线 在IE上输入 http://192.168.1.100/888   即可进入控制LED的画面控制LED

画面如 ： 网页控制LED.jpg

//ip地址修改位置：simple_server.c
//mac地址修改位置：simple_server.c
//SPI , cs修改位置： SPI.c
连线：
STM32f103VBT6    enc28J60
 //PC1 ------------INT	  //不接
 //PC2 ------------WOL	 //不接
  PC10-----------RESET	  
 PC12---------CS	   //PC12 上拉输出模式
 //SPI1-
	PA5-----------SCK		//输出模式
	PA6-----------SO		//输入模式
	PA7-----------SI		//输出模式