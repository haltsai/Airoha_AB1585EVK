import csv
import sys
import time

if len(sys.argv) != 3:
    print("audio dump id parse failed: no parse file and csv path param!")
    exit(-1)

parse_file_path = sys.argv[1]
csv_save_path   = sys.argv[2]
#new_version     = str(time.localtime().tm_year)+('0' if time.localtime().tm_mon < 10 else '')+str(time.localtime().tm_mon)+str(time.localtime().tm_mday)
new_version     = "1"
old_version     = ""

def alter(file_path,old_str,new_str): #str replace in file
     file_data = ""
     with open(file_path, "r") as f:
         for line in f:
             if old_str in line:
                 line = line.replace(old_str,new_str)
             file_data += line
     with open(file_path,"w") as f:
         f.write(file_data)

try:
    read_file  = open(parse_file_path,'r')
    write_file = open(csv_save_path + '/audio_dump_id.csv','w')

    csv_writer = csv.writer(write_file)
    lines      = read_file.readlines()
    csv_writer.writerow(["DUMP_ID_VERSION", new_version])

    for i in range(len(lines)):
        if "typedef enum" in lines[i]:
            start_line = i

        if "DUMP_ID_VERSION" in lines[i]:  #get DUMP_ID_VERSION value in dsp_dump.h
            line        = lines[i].strip('\n').strip()
            old_version = line[line.find('(')+1 : -1].strip()

        if "DSP_DATADUMP_MASK_BIT;" in lines[i]: #parse audio dump id to csv
            for j in range(start_line+3,i):
                line = lines[j].strip('\n').strip()
                if len(line) and "DSP_DATADUMP_MAX_BIT" not in line:
                    dump_name = line[ : line.find('=')].strip()
                    dump_id   = line[line.find('=')+1 : line.rfind(',')].strip()
                    csv_writer.writerow([dump_name, dump_id])
            break
    read_file.close()
    write_file.close()

    #if len(old_version) and int(new_version) != int(old_version):
    #    alter(parse_file_path,old_version,new_version) #runtime replace DUMP_ID_VERSION value in dsp_dump.h

except Exception as err:
    print("audio dump id parse failed: ",err)