#!/usr/bin/python3

import sys
import argparse
import os
import shutil
import re
from datetime import date
import time
from enum import Enum

dbCmd = "mysql -uroot -ptermo yipies_dev -e "
dbFieldNames = "(XCOORD,YCOORD,SPEED,ANGLE,TIME,VALID,LBATT,ABREAK," \
               "MOTION,IN1,IN2,IN3,IN4,IN5,IN6,IN7,IN8,OUT1,OUT2,OUT3," \
               "OUT4,OUT5,OUT6,PFBF,GFTPP,HF,AGEX,AGEY,AGEZ)"

class FieldNum:
    dec = 0
    hex = ''

class Frame:
    class FieldIx(Enum):
        HEADER = 0
        UTC = 1
        LATITUDE = 2
        LONGITUDE = 3
        SPEED = 4
        COURSE = 5
        DATE = 6
        DIGOUTIN = 7
        HOARD = 8
        PQTY = 9
        HUM = 10
        X = 11
        Y = 12
        Z = 13
        BATCHR = 14

    class FlagIx(Enum):
        VALID = 0
        LBATT = 1
        ABREAK = 2
        MOTION = 3

    Id = '!'
    Mark = '|'
    Sep = ','
    Term = '#'
    Types = {'0': 'SGP', '1': 'MSG'}
    FlagValid = {0: 'v', 1: 'a'}
    FlagMotion = {0: 's', 1: 'm'}
    DigInOutValue = {0: 'o', 1: 'c'}
    header = ''
    utc = ''
    latInd = ''
    latitude = ''
    longInd = ''
    longitude = ''
    speed = ''
    course = ''
    date = ''
    digOutIn = ''

    class Header:
        type = '-'
        imei = '---------------'
        class flags(FieldNum):
            pass

    class Dig:
        class DigIn(FieldNum):
            pass
        class DigOut(FieldNum):
            pass

    class Hoard(FieldNum):
        pass

    class Pqty(FieldNum):
        pass

    class Hum(FieldNum):
        pass

    class X(FieldNum):
        pass

    class Y(FieldNum):
        pass

    class Z(FieldNum):
        pass

    class BatChr(FieldNum):
        pass

    class time:
        year = '----'
        month = '--'
        day = '--'
        hour = '--'
        min = '--'
        sec = '--'
        entire = '--------'

    class db:
        pass

    def __init__(self, rawFrame):
        fields = rawFrame.split(',')
        self.header = fields[self.FieldIx.HEADER.value]
        self.Header.type = self.header[1]
        self.Header.flags.hex = self.header[3:5]
        self.Header.imei = self.header[5:]
        self.utc = fields[self.FieldIx.UTC.value]
        self.latitude = fields[self.FieldIx.LATITUDE.value]
        self.longitude = fields[self.FieldIx.LONGITUDE.value]
        self.speed = fields[self.FieldIx.SPEED.value]
        self.course = fields[self.FieldIx.COURSE.value]
        self.date = fields[self.FieldIx.DATE.value]
        self.digOutIn = fields[self.FieldIx.DIGOUTIN.value]
        self.Hoard.hex = fields[self.FieldIx.HOARD.value]
        self.Pqty.hex = fields[self.FieldIx.PQTY.value]
        self.Hum.hex = fields[self.FieldIx.HUM.value]
        self.X.hex = fields[self.FieldIx.X.value]
        self.Y.hex = fields[self.FieldIx.Y.value]
        self.Z.hex = fields[self.FieldIx.Z.value]
        self.BatChr.hex = fields[self.FieldIx.BATCHR.value][0]
        self.Dig.DigOut.hex = self.digOutIn[:2]
        self.Dig.DigIn.hex = self.digOutIn[2:]
        self.latInd = self.latitude[0]
        self.longInd = self.longitude[0]
        self.Hoard.dec = int(self.Hoard.hex, 16)
        self.Pqty.dec = int(self.Pqty.hex, 16)
        self.Hum.dec = int(self.Hum.hex, 16)
        self.X.dec = int(self.X.hex, 16)
        self.Y.dec = int(self.Y.hex, 16)
        self.Z.dec = int(self.Z.hex, 16)
        self.BatChr.dec = int(self.BatChr.hex, 16)
        self.Dig.DigOut.dec = int(self.Dig.DigOut.hex, 16)
        self.Dig.DigIn.dec = int(self.Dig.DigIn.hex, 16)
        self.Header.flags.dec = int(self.Header.flags.hex, 16)

    def mkTime(self):
        self.time.day = self.date[:2]
        self.time.month = self.date[2:4]
        self.time.year = '20' + self.date[4:]
        self.time.hour = self.utc[:2]
        self.time.min = self.utc[2:4]
        self.time.sec = self.utc[4:]
        self.time.entire = self.time.year + self.time.month + self.time.day + \
                           self.time.hour + self.time.min + self.time.sec
        return self.time.entire

    def showInFrame(self):
        print("\nParsed input frame")
        print("------------------")
        print("type      = {0:s} ({1:s})".format(self.Header.type, 
                                                 self.Types[self.Header.type]))
        print("flags     = 0x{0:s} ({1:d})".format(self.Header.flags.hex, 
                                                 self.Header.flags.dec))
        print("imei      = {0:s}".format(self.Header.imei))
        print("utc       = {0:s} (HHMMSS)".format(self.utc))
        print("latitude  = {0:s}".format(self.latitude))
        print("longitude = {0:s}".format(self.longitude))
        print("speed     = {0:s}".format(self.speed))
        print("course    = {0:s}".format(self.course))
        print("date      = {0:s} (DDMMYY)".format(self.date))
        print("digOut    = 0x{0:s} ({1:d})".format(self.Dig.DigOut.hex, 
                                                 self.Dig.DigOut.dec))
        print("digIn     = 0x{0:s} ({1:d})".format(self.Dig.DigIn.hex, 
                                                 self.Dig.DigIn.dec))
        print("hoard     = 0x{0:s} ({1:d})".format(self.Hoard.hex, 
                                                 self.Hoard.dec))
        print("pqty      = 0x{0:s} ({1:d})".format(self.Pqty.hex, 
                                                   self.Pqty.dec))
        print("hum       = 0x{0:s} ({1:d})".format(self.Hum.hex, self.Hum.dec))
        print("x         = 0x{0:s} ({1:d})".format(self.X.hex, self.X.dec))
        print("y         = 0x{0:s} ({1:d})".format(self.Y.hex, self.Y.dec))
        print("z         = 0x{0:s} ({1:d})".format(self.Z.hex, self.Z.dec))
        print("batchg    = 0x{0:s} ({1:d})".format(self.BatChr.hex, 
                                                 self.BatChr.dec))

    def mkDbFields(self):
        self.db.xcoord = self.longitude
        self.db.ycoord = self.latitude
        self.db.speed = self.speed
        self.db.time = self.mkTime()
        self.db.valid = self.FlagValid[getBitValue(self.Header.flags.dec, 0)]
        self.db.lbatt = getBitValue(self.Header.flags.dec, 3)
        self.db.abreak = self.BatChr.dec
        self.db.motion = self.FlagMotion[getBitValue(self.Header.flags.dec, 2)]
        self.db.in1 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 0)]
        self.db.in2 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 1)]
        self.db.in3 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 2)]
        self.db.in4 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 3)]
        self.db.in5 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 4)]
        self.db.in6 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 5)]
        self.db.in7 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 6)]
        self.db.in8 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 7)]
        self.db.out1 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 0)]
        self.db.out2 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 1)]
        self.db.out3 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 2)]
        self.db.out4 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 3)]
        self.db.out5 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 4)]
        self.db.out6 = self.DigInOutValue[getBitValue(self.Dig.DigOut.dec, 5)]
        self.db.pfbf = self.Pqty.dec
        self.db.gftpp = self.Hoard.dec
        self.db.hf = self.Hum.dec
        self.db.agex = self.X.dec
        self.db.agey = self.Y.dec
        self.db.agez = self.Z.dec

        # unused fields
        self.db.history = getBitValue(self.Header.flags.dec, 1)

    def showdbFields(self):
        print("\ndb field values")
        print("---------------")
        print("xcoord = {0:s}".format(self.db.xcoord))
        print("ycoord = {0:s}".format(self.db.ycoord))
        print("speed  = {0:s}".format(self.db.speed))
        print("time   = {0:s}".format(self.db.time))
        print("valid  = {0:s}".format(self.db.valid))
        print("lbatt  = {0:d}".format(self.db.lbatt))
        print("abreak = {0:d}".format(self.db.abreak))
        print("motion = {0:s}".format(self.db.motion))
        print("in1    = {0:s}".format(self.db.in1))
        print("in2    = {0:s}".format(self.db.in2))
        print("in3    = {0:s}".format(self.db.in3))
        print("in4    = {0:s}".format(self.db.in4))
        print("in5    = {0:s}".format(self.db.in5))
        print("in6    = {0:s}".format(self.db.in6))
        print("in7    = {0:s}".format(self.db.in7))
        print("in8    = {0:s}".format(self.db.in8))
        print("out1   = {0:s}".format(self.db.out1))
        print("out2   = {0:s}".format(self.db.out2))
        print("out3   = {0:s}".format(self.db.out3))
        print("out4   = {0:s}".format(self.db.out4))
        print("out5   = {0:s}".format(self.db.out5))
        print("out6   = {0:s}".format(self.db.out6))
        print("pfbf   = {0:d}".format(self.db.pfbf))
        print("gftpp  = {0:d}".format(self.db.gftpp))
        print("hf     = {0:d}".format(self.db.hf))
        print("agex   = {0:d}".format(self.db.agex))
        print("agey   = {0:d}".format(self.db.agey))
        print("agez   = {0:d}".format(self.db.agez))

    def getdbCmdValue(self):
        value = "'{0:s}',".format(self.db.xcoord) + \
                "'{0:s}',".format(self.db.ycoord) + \
                "'{0:s}',".format(self.db.speed) + \
                "'{0:s}',".format(self.db.time) + \
                "'{0:s}',".format(self.db.valid) + \
                "'{0:d}',".format(self.db.lbatt) + \
                "'{0:d}',".format(self.db.abreak) + \
                "'{0:s}',".format(self.db.motion) + \
                "'{0:s}',".format(self.db.in1) + \
                "'{0:s}',".format(self.db.in2) + \
                "'{0:s}',".format(self.db.in3) + \
                "'{0:s}',".format(self.db.in4) + \
                "'{0:s}',".format(self.db.in5) + \
                "'{0:s}',".format(self.db.in6) + \
                "'{0:s}',".format(self.db.in7) + \
                "'{0:s}',".format(self.db.in8) + \
                "'{0:s}',".format(self.db.out1) + \
                "'{0:s}',".format(self.db.out2) + \
                "'{0:s}',".format(self.db.out3) + \
                "'{0:s}',".format(self.db.out4) + \
                "'{0:s}',".format(self.db.out5) + \
                "'{0:s}',".format(self.db.out6) + \
                "'{0:d}',".format(self.db.pfbf) + \
                "'{0:d}',".format(self.db.gftpp) + \
                "'{0:d}',".format(self.db.hf) + \
                "'{0:d}',".format(self.db.agex) + \
                "'{0:d}',".format(self.db.agey) + \
                "'{0:d}'".format(self.db.agez)
        return value

    def dbCmdInsert(self):
        cmd = dbCmd + "\"insert into mobile_" + self.Header.imei + " " + \
              dbFieldNames + " values (" + self.getdbCmdValue() + ")\""
        return cmd

parser = argparse.ArgumentParser(
            add_help=True,
            formatter_class=argparse.RawDescriptionHelpFormatter,
            description=
            "Process Yipies frames from a backup file and write results " + \
            "in a file, whose name is the same as the input file plus the " + \
            "suffix \'.cnv\'.\n" + \
            "The resulting file is located in the same directory of the " + \
            "input file.\n\n" + \
            "The input frames must be in compliance with:\n"
            "!<type>|<flags:2><imei:15>,<utc (HHMMSS)>," + \
            "<latitude>,<longitude>,<speed>,<course>,<date (DDMMYY)>," + \
            "<digOut:2><digIn:2>,<hoard>,<pqty>,<hum>,<x>,<y>,<z>,<batchg>" + \
            "\n\n<flags> is a bit-field of 1 byte width:\n" + \
            "\t[0:0]: validity of GPS location\n" + \
            "\t[1:1]: history or current frame\n" + \
            "\t[2:2]: motion detection\n" + \
            "\t[3:3]: battery status\n" + \
            "\n\nFor example:\n" + \
            "!0|09355826018345180,130044,-37.849640,-059.010879,0.0132," + \
            "156,110920,00FC,0000,00,00,FFFF,0000,0000,0" + \
            "\n\nThe db fields are mapped to input frame fields " + \
            "according to:\n" + \
            "\txcoord <-> longitude\n" + \
            "\tycoord <-> latitude\n" + \
            "\tspeed  <-> speed\n" + \
            "\ttime   <-> date(YYYYMMDD) + utc(HHMMSS)\n" + \
            "\tvalid  <-> flags[0:0]. Values 'v' -> 0, 'a' -> 1\n" + \
            "\tlbatt  <-> flags[3:3]\n" + \
            "\tabreak <-> batchg\n" + \
            "\tmotion <-> flags[2:2]. Values 's' -> 0, 'm' -> 1\n" + \
            "\tin1    <-> digIn[0:0]. Values 'o' -> 0, 'c' -> 1\n" + \
            "\tin2    <-> digIn[0:1]\n" + \
            "\tin3    <-> digIn[0:2]\n" + \
            "\tin4    <-> digIn[0:3]\n" + \
            "\tin5    <-> digIn[0:4]\n" + \
            "\tin6    <-> digIn[0:5]\n" + \
            "\tin7    <-> digIn[0:6]\n" + \
            "\tin8    <-> digIn[0:7]\n" + \
            "\tout1   <-> digOut[0:0]. Values 'o' -> 0, 'c' -> 1\n" + \
            "\tout2   <-> digOut[1:1]\n" + \
            "\tout3   <-> digOut[2:2]\n" + \
            "\tout4   <-> digOut[3:3]\n" + \
            "\tout5   <-> digOut[4:4]\n" + \
            "\tout6   <-> digOut[5:5]\n" + \
            "\tpfbf   <-> pqty\n" + \
            "\tgftpp  <-> hoard\n" + \
            "\thf     <-> hum\n" + \
            "\tagex   <-> x\n" + \
            "\tagey   <-> y\n" + \
            "\tagez   <-> z\n"
            )

parser.add_argument('ifile', action="store", metavar='<in-file-path>',
                    help='file of frames. In general, it is the '
                    'output of frameconv program')
parser.add_argument('--verbose', '-v', action="store", metavar='<level>', 
                    default=0, type=int, choices=range(1, 3), 
                    help='be verbose. Select a verbosity level from 1 to 2')
parser.add_argument('--log', '-l', action="store_true", default=False,
                    help='add the input frame to the output file')

def getBitValue(value, bitIndex):
    return (value >> bitIndex) & 1

def processFile(ifile):
    fcFile = os.path.expanduser(ifile)
    outFile = fcFile + ".cnv"
    nFrames = 0
    with open(fcFile) as infil:
        with open(outFile, "w") as outfil:
            for line in infil:
                if args.verbose > 0:
                    print('>> ' + line[:-len('\n')])
                if args.log == True:
                    outfil.write('# ' + line)

                frame = Frame(line)
                frame.mkDbFields()

                cmd = frame.dbCmdInsert()
                if args.verbose > 0:
                    print('<< ' + cmd)

                if args.verbose > 1:
                    frame.showInFrame()
                    frame.showdbFields()

                outfil.write(cmd)
                nFrames += 1

            if args.verbose > 0:
                print("\nProcessed {0:d} frames".format(nFrames))
            outfil.write('\n')  # add an output file terminator

if __name__ == "__main__":
    try:
        args = parser.parse_args(sys.argv[1:])
        processFile(args.ifile)
        
    except IOError as msg:
        parser.error(str(msg))
