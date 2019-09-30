/**
 *  \file       eeprom.c
 *  \brief      EEPROM M95xxx device controller.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.27  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "eeprom.h"
#include "m95xx0.h"
#include "eeprom_spi.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
static void writeEnable(void);
static void writeDisable(void);
static void waitStandbyState(void);
static void writePage(uint8_t* p, uint16_t addr, uint16_t qty);

/* ---------------------------- Local functions ---------------------------- */

/**
  * @brief  Enables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
static void
writeEnable(void)
{
    uint8_t command = EEPROM_WREN;

    eeprom_spi_select();

    eeprom_spi_write_byte(command);

    eeprom_spi_deselect();
}

/**
  * @brief  Disables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
static void
writeDisable(void)
{
    uint8_t command = EEPROM_WRDI;

    eeprom_spi_select();

    eeprom_spi_write_byte(command);

    eeprom_spi_deselect();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag
  *         in the EEPROM's status register and loop until write 
  *         operation has completed.
  *
  * @param  None
  * @retval None
  */
static void
waitStandbyState(void)
{
    uint8_t command = EEPROM_RDSR;
    uint8_t status;

    eeprom_spi_select();

    // Send "Read Status Register" instruction
    eeprom_spi_write_byte(command);

    // Loop as long as the memory is busy with a write cycle
    do
    {
        eeprom_spi_read_byte(&status);

    } while ((status & EEPROM_WIP_FLAG) != 0);

    eeprom_spi_deselect();
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle
  *         (Page WRITE sequence).
  *
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  p: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  addr: EEPROM's internal address to write to.
  * @param  qty: number of bytes to write to the EEPROM, must be equal
  *         or less than "EEPROM_PAGESIZE" value.
  * @retval none
  */
static void 
writePage(uint8_t* p, uint16_t addr, uint16_t qty)
{
    uint8_t header[3];

    header[0] = EEPROM_WRITE;   // Send "Write to Memory" instruction
    header[1] = addr >> 8; // Send 16-bit address
    header[2] = addr;

    writeEnable();

    eeprom_spi_select();

    eeprom_spi_write(header, 3);

    eeprom_spi_write(p, qty);

    eeprom_spi_deselect();

    writeDisable();

    waitStandbyState();
}

/* ---------------------------- Global functions --------------------------- */
/**
 * @brief Init EEPROM SPI
 *
 * @param hspi Pointer to SPI struct handler
 */
void
eeprom_init(void)
{
    eeprom_spi_init();
}


/**
  * @brief  Writes block of data to the EEPROM. In this function, 
  *         the number of WRITE cycles are reduced, 
  *         using Page WRITE sequence.
  *
  * @param  p: pointer to the buffer containing the data to be written
  *         to the EEPROM.
  * @param  addr: EEPROM's internal address to write to.
  * @param  qty: number of bytes to write to the EEPROM.
  * @retval none
  */
void
eeprom_write(uint8_t* p, uint16_t addr, uint16_t qty)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    uint16_t sEE_DataNum = 0;

    Addr = addr % EEPROM_PAGESIZE;
    count = EEPROM_PAGESIZE - Addr;
    NumOfPage =  qty / EEPROM_PAGESIZE;
    NumOfSingle = qty % EEPROM_PAGESIZE;

    if (Addr == 0)
    { /* addr is EEPROM_PAGESIZE aligned  */
        if (NumOfPage == 0)
        { /* qty < EEPROM_PAGESIZE */
            sEE_DataNum = qty;
            writePage(p, addr, sEE_DataNum);
            return;
        }
        else
        { /* qty > EEPROM_PAGESIZE */
            while (NumOfPage--)
            {
                sEE_DataNum = EEPROM_PAGESIZE;
                writePage(p, addr, sEE_DataNum);

                addr +=  EEPROM_PAGESIZE;
                p += EEPROM_PAGESIZE;
            }
            sEE_DataNum = NumOfSingle;
            writePage(p, addr, sEE_DataNum);
        }
    }
    else
    { /* addr is not EEPROM_PAGESIZE aligned  */
        if (NumOfPage == 0)
        { /* qty < EEPROM_PAGESIZE */
            if (NumOfSingle > count)
            { /* (qty + addr) > EEPROM_PAGESIZE */
                temp = NumOfSingle - count;
                sEE_DataNum = count;
                writePage(p, addr, sEE_DataNum);

                addr +=  count;
                p += count;

                sEE_DataNum = temp;
                writePage(p, addr, sEE_DataNum);
            }
            else
            {
                sEE_DataNum = qty;
                writePage(p, addr, sEE_DataNum);
            }
        }
        else
        { /* qty > EEPROM_PAGESIZE */
            qty -= count;
            NumOfPage =  qty / EEPROM_PAGESIZE;
            NumOfSingle = qty % EEPROM_PAGESIZE;

            sEE_DataNum = count;
            writePage(p, addr, sEE_DataNum);

            addr +=  count;
            p += count;

            while (NumOfPage--)
            {
                sEE_DataNum = EEPROM_PAGESIZE;
                writePage(p, addr, sEE_DataNum);

                addr +=  EEPROM_PAGESIZE;
                p += EEPROM_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                sEE_DataNum = NumOfSingle;
                writePage(p, addr, sEE_DataNum);
            }
        }
    }
}

/**
  * @brief  Reads a block of data from the EEPROM.
  *
  * @param  p: pointer to the buffer that receives the data read from EEPROM.
  * @param  addr: EEPROM's internal address to read from.
  * @param  qty: number of bytes to read from the EEPROM.
  * @retval None
  */
void
eeprom_read(uint8_t* p, uint16_t addr, uint16_t qty)
{
    uint8_t header[3];

    header[0] = EEPROM_READ;    // Send "Read from Memory" instruction
    header[1] = addr >> 8;  // Send 16-bit address
    header[2] = addr;

    eeprom_spi_select();

    /* Send addr address byte to read from */
    eeprom_spi_write(header, 3);

    eeprom_spi_read(p, qty);

    eeprom_spi_deselect();
}

/**
  * @brief  Reads current value of EEPROM Status Register
  *
  * @param  None
  * @retval Status Register value
  */
uint8_t
eeprom_readStatusRegister(void)
{
    uint8_t command = EEPROM_RDSR;
    uint8_t status;

    eeprom_spi_select();

    // Send "Read Status Register" instruction
    eeprom_spi_write_byte(command);

    eeprom_spi_read_byte(&status);

    eeprom_spi_deselect();

    return status;
}


/**
  * @brief  Write new value in EEPROM Status Register.
  *
  * @param  regval : new value of register
  * @retval none
  */
void
eeprom_writeStatusRegister(uint8_t regval)
{
    uint8_t command[2];

    command[0] = EEPROM_WRSR;
    command[1] = regval;

    writeEnable();

    eeprom_spi_select();

    // Send "Write Status Register" instruction
    // and Regval in one packet
    eeprom_spi_write(command, 2);

    eeprom_spi_deselect();

    writeDisable();
}

/* ------------------------------ End of file ------------------------------ */

