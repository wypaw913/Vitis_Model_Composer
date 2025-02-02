// ==============================================================
// File generated by Xilinx Model Composer(TM)
// Version: 2019.2
//  
// (c) Copyright 2017 - 2019 Xilinx, Inc. All rights reserved.
//  
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
//  
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//  
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//  
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// ==============================================================

#include <stdint.h>
#include "ap_int.h"
#include "imgproc/xf_magnitude.hpp"
#include "common/xf_structs.hpp"
#include "common/xf_common.hpp"

#pragma XMC INPORT inX,inY
#pragma XMC OUTPORT out
#pragma XMC SUPPORTS_STREAMING

template<int ROWS, int COLS, int NPC>
void GradientMag_XMC(uint16_t inX[ROWS][COLS], 
                     uint16_t inY[ROWS][COLS],
                     uint16_t Out[ROWS][COLS]
                    )
{  
     ap_uint<16> dataX_in;
     ap_uint<16> dataY_in;
     ap_uint<16> data_out;
   
     const int NORM_TYPE = XF_L1NORM;
     const int TYPE = XF_16SC1;
     const int height = ROWS;
     const int width = COLS;
     const bool USE_URAM = false;
     const int bufferSize = 100;
     const int bufferPrefetch = 50;
 
  
    xf::cv::Mat<TYPE,ROWS,COLS,XF_NPPC1> srcX_obj;
    #pragma HLS stream variable=srcX_obj dim=1 depth=1
    
    xf::cv::Mat<TYPE,ROWS,COLS,XF_NPPC1> srcY_obj;
    #pragma HLS stream variable=srcY_obj dim=1 depth=1
    
 
    xf::cv::Mat<TYPE,ROWS,COLS,XF_NPPC1> dst_obj;
    #pragma HLS stream variable=dst_obj dim=1 depth=1
    
    
    #pragma HLS dataflow
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
        #pragma HLS pipeline II=1
            dataX_in = inX[i][j];
            dataY_in = inY[i][j];
            srcX_obj.write(i * COLS + j,(dataX_in));
            srcY_obj.write(i * COLS + j,(dataY_in));
        }
    }
    
     xf::cv::magnitude<NORM_TYPE,XF_16SC1,XF_16SC1,height, width,NPC> (srcX_obj,srcY_obj,dst_obj);


      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
        #pragma HLS pipeline II=1
            data_out = dst_obj.read(i * COLS + j);
            Out[i][j] = data_out;
        }
      }
     
  }