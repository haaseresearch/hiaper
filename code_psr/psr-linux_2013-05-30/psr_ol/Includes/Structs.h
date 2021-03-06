#ifndef STRUCTS_H
#define STRUCTS_H

// FRAME_SIZE_PLUS_2, MAX_CHANNELS; allows Structs.h to stand alone

/*Data pertaining to signal type and characteristics*/
/********************************************************************************/
// Complex data structure
struct cpx {
	DATA_TYPE r;
	DATA_TYPE i;
};
typedef cpx DATA_CPX;
typedef cpx CPX;
// Correlator structure
struct corr {
	DATA_TYPE rr;
	DATA_TYPE ii;
};
typedef corr DATA_CORR;

// Complex data (large) structure
struct big_cpx {
	int r;
	int i;
};

typedef struct CPX_ACCUM {

	int i;	//!< Inphase (real)
	int q;	//!< Quadrature (imaginary)

} CPX_ACCUM;

// Complex sine wave structure
struct cpx_sine {
	short r1;
	short i;
	short ni;
	short r2;
};
typedef cpx_sine MIX;
/********************************************************************************/

struct Acq_Grid
{
	int power[2*ACQ_DOPPLER_BINS+1][ACQ_SAMPLES_PER_MS]; //the raw data
	int SV;		//SV
	int tic;	//updated on this tic
	int x;		//x coordinate
	int y;		//y coordinate
	float psi;	//rotation
	float rho;	//rotation
	int max;	//for color scaling
};

typedef struct Options{
	bool forward;
	bool backward;
}Options;
/*Channels Structs */
/********************************************************************************/
typedef struct PLL {
	float PLLBW;
	float FLLBW;

	float a3;
	float b3;
	float w0p;
	float w0p2;
	float w0p3;

	float a2;
	float w0f;
	float w0f2;
	float gain;
//	double PLLBW;
//	double FLLBW;
//
//	double a3;
//	double b3;
//	double w0p;
//	double w0p2;
//	double w0p3;
//
//	double a2;
//	double w0f;
//	double w0f2;
//	double gain;
}PLL;

struct DLL {
	float Code_Delta;
	float Code_D1;
	float Code_D2;
};
/********************************************************************************/


struct Measurement_S {
	int _1ms_epoch;
	int _20ms_epoch;
	int frame_ms_epoch;

	double code_phase;
	double frac_code_phase;
	double code_rate;

	double frac_carrier_phase;
	double carrier_phase;
	double carrier_rate;

	double float_code_phase;
	double float_code_time;
	double float_carrier_phase;
	double float_carrier_time;

	int frame_z;
	unsigned int tic;
};

/* Acquisition Stuff */
/********************************************************************************/
struct Acq_Result {
	int		SV;
	int		dopp_bin;
	float	code_phase;
	float	doppler;
	float	SNR;
	float   NewSNR;
	int   	CorrMax;
	bool	detected;
	float	Mean;
};

struct Acq_Options {
		int samples_per_ms;		//samples per ms of data
		int resamples_per_ms;	//resampled per ms of data
		int doppler_bins;		//# of doppler bins to search
		int doppler_bin_width;	//doppler bin spacing
		int cticks;				//coherent integration ticks
		int iticks;				//incoherent integration ticks
		int tticks;				//total number of ticks

		double sample_rate;		//sample rate
		double resample_rate;	//resample rate

		char filename[1024];	//filename
		bool log;				//log the acq

};
/********************************************************************************/
/* PipeWall Stucts */
/********************************************************************************/
typedef struct PipeWall_2_Navigator_S {
	int SV;
	int Chan;
	int Subframe_num;
	long Zc;
	bool Navigate;
	Measurement_S aMeasurement;
}PipeWall_2_Navigator_S;

typedef struct PipeWall_2_Ephemeris_S {
	int SV;
	int Chan;
	int Subframe_num;
	unsigned long Subframe[FRAME_SIZE_PLUS_2];
}PipeWall_2_Ephemeris_S;

struct PipeWall_2_GUI_S {
	int		SV;
	int		Chan;
	int		subframe;
	float	f_doppler;
	float	CN0;
	int		best_epoch;
	float	Re_prompt_avg;
	long	samples_processed;
	bool	bit_lock;
	bool	frame_lock;
	bool	navigate;
	float	I_avg;
	float	Q_var;
	int		rollover;

	#if (GUI_OUTPUT >= GUI_HIGH)
		int		i_buff[MEASUREMENT_INT]; //inphase
		int		q_buff[MEASUREMENT_INT]; //quadrature
		int     e_buff[MEASUREMENT_INT]; //early
		int		p_buff[MEASUREMENT_INT]; //prompt
		int		l_buff[MEASUREMENT_INT]; //late
	#endif
};

struct PipeWall_2_WaveGen_S {
	int SV;
	int Chan;
	long samples;
	float f_carrier;
	float fine_code_phase;
};

typedef struct PipeWall_Struct {
	int SV;						/* This pipe's SV */
	int Chan;					/* This pipe's channels number */
	PipeWall_2_Navigator_S Pipe_2_Nav;
	PipeWall_2_Ephemeris_S Pipe_2_Ephem;

	#if (GUI_OUTPUT > GUI_NONE)
		PipeWall_2_GUI_S   Pipe_2_GUI;
	#endif

	#if R_WAVEFORM_GEN
		PipeWall_2_WaveGen_S Pipe_2_WaveGen;
	#endif
}PipeWall_Struct;
//***************************************************************************************

struct Clock_Control_S {
	double err;
	double derr; //derivate
	double ierr; //integral
	double perr; //previous
	double kp;
	double ki;
	double kd;
	double output;
};

struct Clock_S {
	double RCOsec;
	unsigned int RCOtic;
	double RCOrate; 		//RCO rate
	double RCOsrate;		//smoothed clock rate
	double RCObias; 		//clock bias
	double RCOsbias; 		//smoothed clock_bias
	double RCOrate_bias;	//integrated rate
	double RCOsrate_bias;	//integrated rate bias smoothed
	double RCO_Carrdopp;	//effective doppler from clock
	double RCO_Codedopp;	//effective doppler from clock
	double GPSsec;			//gps second
	double GPSweek;			//gps week
	double Last_RCObias;	//Previous RCObias correction needed for time tag fixing
	Clock_Control_S RCOcontrol;
};


//---------------------------------------------------------
// Millisecond header struct - based on APL's proposed format for the GRS
// Don't change this!!  Could create breaking errors!  This has to be _exactly_ 64*2 bytes long
#pragma pack(push,2)
#define MSEC_HEADER_FRAME_WORD_0 (0xAA55U)
#define MSEC_HEADER_FRAME_WORD_1 (0x1234U)
struct Msec_Header_S {
	unsigned short FrameWords[2];
	unsigned long GPS_MSOW; // HACK : LITTLE_ENDIAN SPECIFIC
	//unsigned short GPS_MSOW_low;
	//unsigned short GPS_MSOW_high;
	unsigned short GPS_Week;
	unsigned short EventBits1;
	unsigned short OnePPS;
	unsigned short TLM_FSY;
	short Temp_BCR_Board;
	short Temp_BCR_Processor;
	short Temp_RFDC_Board;
	short Temp_RFDC_Oscillator;
	unsigned short RFDC_AGC_Ch12;
	unsigned short RFDC_AGC_Ch3_15V;
	unsigned short OscillatorStatus;
	short Monitor5V;
	short Monitor3p3V;
	short Monitor1p5V;
	short ADC_Input[5];
	unsigned short EventBits2;
	unsigned short Growth[20];
	unsigned long Power_Chan0_Sum; // HACK: LITTLE_ENDIAN SPECIFIC
	unsigned long Power_Chan1_Sum; // HACK: LITTLE_ENDIAN SPECIFIC
	unsigned long Power_Chan2_Sum; // HACK: LITTLE_ENDIAN SPECIFIC
	//unsigned short Power_Chan0_Sum_Low;
	//unsigned short Power_Chan0_Sum_High;
	//unsigned short Power_Chan1_Sum_Low;
	//unsigned short Power_Chan1_Sum_High;
	//unsigned short Power_Chan2_Sum_Low;
	//unsigned short Power_Chan2_Sum_High;
	unsigned short Growth2[13];
	unsigned short Checksum;
};
#pragma pack(pop)
//---------------------------------------------------------

//---------------------------------------------------------
// OL Prediction structure
// TODO finalize the format of this (and be sure the other tools work correctly, too!)
#pragma pack(push,1)
struct OL_Predict_S {
	unsigned short GPS_Week;
	unsigned long GPS_SOW;
	double FreqPredictHz[1000];
};

struct OL_Climate_S {
    unsigned short GPS_Week;
	unsigned long GPS_SOW;
    double FreqClimateHz[1000];
};

struct Bit_Grabber_S {
	unsigned long Z_count; // z_count that corresponds to actual transmission time
	short data_bit[300]; // array of data bits as transmitted (0->+1,1->-1)
};
#pragma pack(pop)
//---------------------------------------------------------

//---------------------------------------------------------
// OL start structure - FRAME based
struct OL_Frame_Start_S {
    int SV;
	int frame_edge_index;
	double f_doppler;
	double frac_code_phase;
	double frac_carrier_phase;
	double carrier_phase_output;
	short sid;
	int z_count;
	unsigned long msow;
};
//---------------------------------------------------------


#endif // STRUCTS_H
