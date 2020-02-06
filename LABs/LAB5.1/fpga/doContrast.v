  module doContrast ( 
resetn,
 ivalid,
 ovalid,
 iready,
 oready,
 x,
 r,
 clock);
input  resetn;
input  ivalid;
input  iready;
input [7:0] x;
input  clock;
output  ovalid;
output  oready;
output [7:0] r;

//wire clk;
//wire reset;

//assign clk = clock;
//assign reset = ~resetn;

contrast_legacy obj (resetn, x,r);

endmodule
