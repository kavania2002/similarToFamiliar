import io
import os
import easyocr
import fitz
import numpy as np
from PIL import Image, ImageDraw
from pdf2image import convert_from_path

telemetry = 0 # 0 for off, 1 for on
progress_bar = 1
debugmode = 0


# get course codes
ccs = open('data/course_codes.csv','r')
# for all pdf file in the folder data/SNA-132x-1830
folderName = 'data/input_files/SNA-132x-1830/'

# output file
# filename_ID_link_filename = 'data/output_files/SNA-132x-1830/filename_ID_link_filename.csv'
# course_taken_ID = 'data/output_files/SNA-132x-1830/course_taken_ID.csv'
filename_ID_link_filename = 'data/output_132xfilename_ID_link_filename.csv'
course_taken_ID_filename = 'data/output-132x-Bipartite.csv'


ccs = ccs.read()
ccs = ccs.splitlines()

# create a set of course codes
ccs_set = set(ccs)

filename_ID = dict()  # storing { id: filename, }
course_taken_ID = dict()     # storing { id: [<course_code>_<sem_id>], }


current_id = 0

def sort_by_y_axis(result):
    return result[0][1], result[0][0]


def draw_progress_bar(progress, barLength = 20):
    # Progress bar
    status = ""
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
        status = "error: progress var must be float\n"
    
    use_character=chr(9608)

    if progress < 0:
        progress = 0

    completed_length = int(round(barLength * progress))
    bar = use_character * completed_length + '-' * (barLength - completed_length)

    print('\t [', bar, ']\t', int(progress*100), "%", status, sep="", end='\n')

    if progress >= 1:
        progress = 1
        status = "Done...\n"


# only get pdf files

fileNameList = os.listdir(folderName)
fileNameList = [f for f in fileNameList if f[-3:] == 'pdf']

if(debugmode): fileNameList = fileNameList[0:1]

for pdf_file in fileNameList:

    pdf_file_path = folderName + pdf_file
    # Open the PDF file
    images = convert_from_path(pdf_file_path)

    # Loop through each page in the PDF and render as an image
    images_list = []
    for image in images:
        # Add the image to the list of images
        images_list.append(image)

    if(telemetry): print("Pages in PDF:", len(images_list), "Name of file:", pdf_file)
    if(telemetry): print("Rendered to images, now running OCR...")

    # Run EasyOCR on each image and extract text
    reader = easyocr.Reader(['en'])  # Create an OCR reader for English text
    texts = []
    for img in images_list:
        # Convert the image to grayscale and then to a numpy array
        img_np = np.array(img.convert('L'))

        # reduce image dimension by 2
        

        # Extract text using EasyOCR
        result = reader.readtext(img_np)


        # Sort the text by their y-coordinate
        # result.sort(key=lambda r: (r[0][0], r[0][1]))
        new_elements = dict()
        for element in result:
            y_center = (element[0][0][1]+element[0][2][1])/2
            while(y_center in new_elements.keys()):
                y_center += 0.00001*y_center
            new_elements[y_center] = element
        

        
        myKeys = list(new_elements.keys())
        myKeys.sort()
        result = []
        for key in myKeys:
            result.append(new_elements[key])


        # # Concatenate the text into a single string
        # text = '\n'.join([res[1] for res in result])
        # texts.append(text)
        for res in result:
            texts.append(res[1])

        # Draw bounding boxes around the detected text
        draw = ImageDraw.Draw(img)
        for res in result:
            bbox = res[0]
            shape = [(bbox[0][0], bbox[0][1]), (bbox[2][0], bbox[2][1])]
            draw.rectangle(shape, outline='red', width=2)

    # Print the extracted text for each page in order from top to bottom
    if(telemetry): print(texts)


    def hasCourse(line):
        # remove spaces
        line = line.replace(" ","")
        
        # sliding window of 6
        for i in range(len(line)-5):
            if(len(line) > 5):
                mystr = line[i:i+6].upper()
                newstr = mystr[0:3].replace("0","O") + mystr[3:6].replace("O","0")
                if newstr in ccs_set:
                    return (newstr).upper()
        
        return None

    def hasSemester(line):
        # remove spaces
        line = line.strip()
        if(line in available_sems):
            return available_sems[line]
        else:
            return None
        
    def hasCalendar(line):
        line = line.strip()
        if(line.__contains__("Calendar")):
            return 1
        else:
            return 0


    # texts = ['Winter Semester 2023', 'Important Note: Attendance for this course will be calculated based on course enrollement date', 'Course', 'Total Session', '#', 'Section', 'Enrol Date', 'Present', 'Absent', 'Present', '[%]', 'Section 2', '21-Nov-2022 21.30', '31', '29', '94%', 'CSE533 Social Network Analysis', '2', 'Section', '24-Nov-2022 09.30', '22', '91%', '1', '20', '2', 'CSE523 Machine Learning', '2', '15', '13', '3', 'Section', '24-Nov-2022 09.34', '2', '87%', 'CSE537 High Speed Computer Architecture', '13', '69%', 'ECO11O Macroeconomics', 'Section 4', '24-Nov-2022 11:18', '9', '4', 'Section 2', '24-Nov-2022 11.41', '23', '5', '82%', 'CSE342 Computer Networks', '5', '28', 'Section', '30-Nov-2022 16.45', '92%', '13', '12', '6', '1', 'BIO1O5 Fundamentals of Environmental Studies', 'ENR3O6 Technical Communication', 'Section 2', '03-Jan-2023 13.27', '12', '10', '83%', '2', '10', '8', 'Section', '12-Jan-2023 21.26', '8', '80%', '2', 'PSY2O5 Evolutionary Psychology', 'Total', '144', '124', '20', '86 %', 'Monsoon Semester 2022', 'Important Note: Attendance for this course will be calculated based on course enrollement date', 'Course', 'Section', 'Enrol Date', 'Total Session', 'Present', 'Present', '#', 'Absent', '[%]', '28', '25', '89%', 'Section', '06-Jun-2022 11.09', '3', 'PSY210 Cognitive Psychology', '02-Jul-2022 12.28', 'Section 2', '15', '15', '100%', '2', 'ENR2O4 Mechanics of Rigid Bodies', 'Section 2', '02-Jul-2022 12.54', '92%', '13', '12', '3', 'ENR2O3 Material Science and Engineering', '02-Jul-2022 13.53', '100%', 'Section 3', '28', '28', 'ENR3O5 Sensors, Instruments and Experimentation', 'Section 1', '18-Jul-2022 10.47', '27', '27', '100%', '5', '0', 'ECE502 VLSI Design', 'Section 2', '19-Jul-2022 17.08', '55', '53', '96%', 'ECE302 Embedded Systems Design', '2', '81', '99%', 'Section 2', '19-Jul-2022 18.45', '82', 'CSE332 Operating Systems', 'Section', '37', '95%', '8', 'MAN111 Conversational Mandarin', '35', '2', '04-Aug-2022 20.06', 'Total', '285', '276', '9', '97 %', 'Winter Semester 2022', 'Important Note: Attendance for this course will be calculated based on course enrollement date', 'Course', 'Section', 'Enrol Date', 'Total Session', 'Absent', 'Present', '#', 'Present', '[%]', '96%', 'MAT277 Probability and Stochastic Processes', 'Section 2', '22-Nov-2021 07.34', '27', '26', '28', '24', '86%', '22-Nov-2021 07:34', 'Section', 'CSE2OO Design and Analysis of Algorithms', '2', '1', '22-Nov-2021 07.35', 'Section 2', '42', '83%', '3', 'CSE2O6 Computer Organization and Architecture', '35', 'Section 1', '22-Nov-2021 07.35', '42', '35', '83%', 'CSE250 Database Management Systems', '5', '5', 'Section', '22-Nov-2021 07.37', '24', '83%', '29', '1', 'ENR2OO Design, Innovation and Making', 'Section 1', '22-Nov-2021 07:39', '85%', '4', 'DES2O2 Interaction Design', 'UX and UI', '26', '22', '25-Nov-2021 09.03', 'Section 1', '28', '24', '4', '86%', 'MAT 485 Introduction to Quantum Computing', '0', '%', '8', '0', 'VOL1OO Engagement with Society', '06-May-2022 10.53', 'Total', '222', '190', '32', '86 %', 'Monsoon Semester 2021', 'Important Note: Attendance for this course will be calculated based on course enrollement date', '#', 'Absent', 'Section', 'Course', 'Enrol Date', 'Total Session', 'Present', 'Present', '[%]', 'Section', '38', '87%', 'PHY112 Electromagnetic Theory', '10-May-2021 16.30', '33', '5', 'Section 2', '35', '34', '97%', '2', 'ECE209 Digital Design', '10-May-2021 16.50', '0', 'Section', '29', '29', '100%', '10-May-2021 16.55', '3', 'ECE210 Signals and Systems', 'Section 2', '41', '39', '95%', 'CSE2O5 Data Structures', '10-May-2021 16.58', '2', '29', '0', '5', 'Section', '17.07', '29', '100%', '10-May-2021', 'MAT248 Applied Linear Algebra', 'Section', '28', '28', '100%', '0', '10-May-2021 17:08', 'PSY101 Introduction to Psychology', 'Total', '8', '96 %', '200', '192', 'Attendance Calendar', '(Green: Present', 'Red: Absent; Orange: Holiday/Event/Exam, Orange: Justified Leave Absence)', 'March 2023', 'month', 'week', 'today', 'day', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun', 'Mon', 'I8a CSE533 Social Network', '5', 'I10a CSE523 Machine Learning', '10a CSE523 Machine Learning', 'Analysis', '1pECO11O Macroeconomics', 'Ip CSE342 Computer', 'Networks', '9.30a CSE537 High Speed', 'IComputer Architecture', '2p CSE342 Computer', 'NNetworks', '2.30p CSE342 Computer', 'Networks', '11', '12', '10', '8', 'I8a CSE533 Social Network', 'I8a CSE533 Social Network', '(10a CSE523 Machine Learning', '10a CSE523 Machine Learning', 'Analysis', 'Analysis', '1pECO11O Macroeconomics', '(9.30a CSE537 High Speed', 'I9.30a CSE537 High Speed', '3p CSE533 Social Network', 'IComputer Architecture', 'IComputer Architecture', 'Analysis', '4pBIO1O5 Fundamentals of', 'I2:30p CSE342 Computer', '4p CSE533 Social Network', 'NNetworks', 'Environmental Studies', 'JAnalysis', 'l4p BIO105 Fundamentals of', 'IEnvironmental Studies', 'l.30p PSY205 Evolutionary', 'PPsychology', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31']

    available_sems = {
        "Winter Semester 2023": 0,
        "Monsoon Semester 2022": 1,
        "Winter Semester 2022":2,
        "Monsoon Semester 2021": 3,
        "Winter Semester 2020":4,
        "Monsoon Semester 2020":5,
    }

    current_sem = -1    ## store the index of sem according to available_sems

    this_pdf_courses = []

    for line in texts:
        # if line has course, change current course to other
        sem_test = hasSemester(line)
        if(type(sem_test) != type(None)):
            current_sem = sem_test
            continue

        course_test = hasCourse(line)
        if(type(course_test) != type(None)):
            this_pdf_courses.append(str(course_test) + "_" + str(current_sem))
            continue

        if(hasCalendar(line)):
            break

    if(telemetry): print(this_pdf_courses)

    filename_ID[current_id] = pdf_file
    course_taken_ID[current_id] = this_pdf_courses
    current_id += 1

    if(telemetry): print("Done with " + pdf_file)
    if(progress_bar): draw_progress_bar(current_id/len(fileNameList), barLength = 20)


# write to file at end
with open(filename_ID_link_filename, 'w') as fp:
    for key in filename_ID.keys():
        fp.write(str(key) + "," + filename_ID[key] + "\n")

with open(course_taken_ID_filename, 'w') as fp:
    for key in course_taken_ID.keys():
        if(len(course_taken_ID[key]) == 0):
            print("No course taken for " + str(key))
            continue
        for course in course_taken_ID[key]:
            fp.write(str(key) + "," + course + "\n")                 
