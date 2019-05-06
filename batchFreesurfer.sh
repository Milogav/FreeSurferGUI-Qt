#!/bin/bash

###########################FUNCTION DECLARATION######################################

####################################################################################################################

##################################### SCRIPT ##############################
t1Path=$1   #######
t2Path=$2
export FREESURFER_HOME=$3
outDIR=$4 #######
sujName=$5

source $FREESURFER_HOME/SetUpFreeSurfer.sh

ORANGE='<font color="DarkOrange">'
RED='<font color="Red">' ##HTML color codes for echo output in qtextedit as HTML text
NC='</font>'
BLUE='<font color="blue">'

######## CHECK IMAGE FILES MODALITIES AND PERFORM DICOM TO NII CONVERSION #############
echo -e "<br>${ORANGE}Accesing file: $t1Path ${NC}<br>"

echo -e "${RED}Storing results in: ${BLUE}$outDIR${NC}<br>"
export SUBJECTS_DIR=$outDIR
    
####### FREESURFER PARCELATION
if [ -f $t2Path ];then ### if available, use t2 volume to improve pial surfaces
     echo -e "${RED}T2 available for pial surfaces improvement ... $t2Path${NC}<br>"
     echo -e "${ORANGE}LAUNCHING COMMAND: recon-all -i $t1Path -s $sujName -T2 $t2Path -T2pial -openmp 4 -all${NC}<br>"
     recon-all -i $t1Path -s $sujName -T2 $t2Path -T2pial -openmp 4 -all > $outDIR/FreeSurferLog.txt 2>&1
     mv $outDIR/FreeSurferLog.txt $outDIR/$sujName
else
     echo -e "${RED}LAUNCHING COMMAND:${ORANGE} recon-all -i $t1Path -s $sujName -openmp 4 -all${NC}<br>"
     recon-all -i $t1Path -s $sujName -openmp 4 -all > $outDIR/FreeSurferLog.txt 2>&1
     mv $outDIR/FreeSurferLog.txt $outDIR/$sujName
fi  
                
echo -e "${ORANGE}\nDONE${NC}" 	 

