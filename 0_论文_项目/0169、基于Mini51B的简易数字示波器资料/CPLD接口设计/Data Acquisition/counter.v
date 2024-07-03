module counter(clkin,clkout);
input clkin;
output clkout;
reg [1:0]i;
reg clkout;

always@(posedge clkin)
begin
	i<=i+1;
	if(i==3)clkout=!clkout;
end
endmodule
