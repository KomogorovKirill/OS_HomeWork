#!/usr/bin/env python3
import sys
from struct import unpack, pack
if len(sys.argv) != 2:
    print(f"Using: {sys.argv[0]} <name_image>")
    exit(1)

try:
    with open(sys.argv[1], 'rb') as fd:
        data = fd.read()
        fd.close()
except:
    print(f"Error! File {sys.argv[1]} not found!")
    exit(1)
if not data:
    print("Error! ")
    exit(1)
        
SectorSize, ClusterSize, addrMFT, addrMFTMirr, SizeRecordMFT, sig = unpack("=11xHB34xQQB445xH", data[:512])

if sig != 0xAA55:
    print("Неверная сигнатура загрузочного сектора")
    exit(1)

if SizeRecordMFT > 0x80:
    SizeRecordMFT = -1 * (256 - SizeRecordMFT)
    SizeRecordMFT = 2 << ~SizeRecordMFT
else:
    SizeRecordMFT *= ClusterSize * SectorSize

addrMFT *= SectorSize * 8
addrMFTMirr *= SectorSize * 8

print(f"Длина сектора:                                 {SectorSize} байт")
print(f"длина кластера:                                {ClusterSize} секторов = {ClusterSize * SectorSize} дайт")
print(f"Адрес MFT:                                     {hex(addrMFT)}")
print(f"Адрес MFTMirr:                                 {hex(addrMFTMirr)}")
print(f"Длина одной записи MFT:                        {SizeRecordMFT} байт")

filesign, markers, markersnum, transactnum, firstattr = unpack("=IHHQ4xH", data[addrMFT:addrMFT+22])
filesign = pack("I", filesign)
filesign = unpack(">I", filesign)[0]

print(f"Сигнатура FILE:                                 {hex(filesign)}")
print(f"Смещение массива маркеров:                      {hex(markers)}")
print(f"Колличество элементов в массиве маркеров:       {markersnum}")
print(f"Номер журнала транзакций:                       {transactnum}")
print(f"смещение до первого атрибута:                   {firstattr}")

checkend = 0
numAttr = 0
while checkend != 0xFFFFFFFF:
    numAttr += 1
    print(f"\nИнформация о {numAttr} атрибуте")
    attrType, lenAttr, checkres, attrNameLen, attrNameMov, flags, attrIdent = unpack("=IIBBHHH", data[addrMFT+firstattr:addrMFT+firstattr+16])
    if checkres !=1:
        print("Атрибут резидентный")
        contLen, contMov = unpack("=IH", data[addrMFT+firstattr+16:addrMFT+firstattr+22])
    else:
        print("Атрибут нерезидентный")
        contLen, contMov == None
        virclusbeg, virclusend, series, block, unused, attrSizeBold, factAttrSize, initAttrSize, afterComprSize = unpack("=QQHHIQQQQ", data[addrMFT+firstattr+16:addrMFT+firstattr+72])
        print("Доп. параметры нерезидентного атрибута: ")
        print(f"\tНомер начального виртуального кластера:     {virclusbeg}")
        print(f"\tКонечный номер виртуального кластера:       {virclusend}")
        print(f"\tСмещение списка серий:                      {series}")
        print(f"\tРазмер блока сжатия:                        {block}")
        print(f"\tНе используется:                            {unused}")
        print(f"\tВыделенный размер содержимого атрибута:     {attrSizeBold}")
        print(f"\tФактический размер атрибута:                {factAttrSize}")
        print(f"\tИнициализированный размер атрибута:         {initAttrSize}")
        print(f"\tРазмер атрибута после сжатия:               {afterComprSize}")
    print(f"Тип атрибута:                               {hex(attrType)}")
    print(f"Длина атрибута:                             {lenAttr}")
    print(f"Флаг резидентности:                         {checkres}")
    print(f"Длина имени аттрибута:                      {attrNameLen}")
    print(f"Смещение имени атрибута:                    {attrNameMov}")
    print(f"Флаги:                                      {flags}")
    print(f"Идентификатор атрибута:                     {attrIdent}")
    print(f"Длина содержимого:                          {contLen}")
    print(f"Смещение содержимого:                       {contMov}")
    corr, checkend = unpack(f"=BI", data[addrMFT+firstattr+lenAttr-1:addrMFT+firstattr+lenAttr+4])
    addrMFT += lenAttr
