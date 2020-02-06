//
// outv = m * inv + n
//
//	m is the slope. we will use a fraction to compute it (like 3/2)
//  n is the offset, which we will derive by forcing to have the response
//	  centered so...
//
//		128 = 3 / 2 * 128 + n    ---> n = 128 - 128(3/2) = -64 

//
//           m is the slope. we will use a fraction to compute it (like 3/2)
//  n is the offset, which we will derive by forcing to have the response
//             centered so...
//
//                         128 = 3 / 2 * 128 + n    ---> n = 128 - 128(3/2) = -64 
`define FRAC_NUM 3
`define FRAC_DEN 2
`define N 64

module contrast_legacy(input resetn, 
              input [7:0] inv , 
              output wire [7:0] outv ); 

wire [15:0] s1;
wire [15:0] s2;
wire [15:0] s3;
assign s1 = inv * `FRAC_NUM;
assign s2 = s1 / `FRAC_DEN;
assign s3 = s2 - `N; 

assign outv =  (s2 < `N) ? 0 : (s3 > 255) ? 255 : s3;

endmodule
