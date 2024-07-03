unsigned int chksum(unsigned char *check,unsigned int size)
{
	unsigned int	i;
	unsigned long	sum = 0;
	unsigned int	*ptr = (unsigned int *)check;

	for (i = 0; i < (size) / 2; i++)
	{
		sum += *ptr++;
	}
	if (size & 0x01)		//��ʾ����Ϊ����
	{
		sum += ((*ptr)&0xff);
	}
	sum = (sum & 0xffff) + ((sum>>16)&0xffff);//��16λ�͵�16λ���
	if(sum&0xffff0000)
	{//��ʾ�н�λ
		sum++;
	}
	return ((unsigned int)((sum)&0xffff));
}

unsigned int Swap16U(unsigned int data16u)
{
	unsigned int	a;
	a = data16u >> 8;
	a += ((data16u & 0xFF) << 8);
	return	a;
}

unsigned long Add16U(unsigned int a, unsigned int b)
{
	unsigned long	tmp;
	tmp = (((unsigned long)Swap16U(a)) << 16) + ((unsigned long)Swap16U(b));
	return	tmp;
}
