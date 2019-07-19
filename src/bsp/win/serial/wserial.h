#ifndef __WSERIAL_H__
#define __WSERIAL_H__

/*
 * 		serial.h
 * 			Hardware dependent
 * 			serial port functions
 */

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	PAR_NONE, PAR_ODD, PAR_EVEN, PAR_MARK, PAR_SPACE
};

enum
{
	STOP_1, STOP_1HALF, STOP2
};

typedef struct
{
	void (*rx)( unsigned char byte );
	void (*serial_connect)( void );
	void (*serial_disconnect)( void );
	void (*serial_data_sent)( void );
	void (*serial_ring)( void );
	void (*serial_cd_on)( void );
	void (*serial_cd_off)( void );
} SERIAL_CBACK_T;

void init_serial_hard( int device_no, SERIAL_CBACK_T *p );
void connect_serial( int device_no );
void tx_data( int device_no, char byte );
void tx_n_data( int device_no, char *p, char qty );
void reset_rts( int device_no );
void set_rts( int device_no );
void reset_dtr( int device_no );
void set_dtr( int device_no );
void disconnect_serial( int device_no );
void deinit_serial_hard( int device_no );


void init_serial( void (*prcv)(unsigned char byte) );
void close_mserial( void );

void ser_put_string( int ch, char * p );
void ser_printf( char *fmt, ... );

#ifdef __cplusplus
}
#endif

#endif

