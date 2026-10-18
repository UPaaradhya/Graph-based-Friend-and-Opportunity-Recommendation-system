// =====================================================================
// NEW DATASET: 100 users, 75 opportunities, friendships, received courses
// Run with the loader:
//    .\load.exe 'YOUR_NEO4J_PASSWORD' neo4j_100_users.cypher
// User IDs 1 to 100. Passwords are in user_credentials_100.csv
// =====================================================================

// ---------- STEP 1: delete everything ----------
MATCH (n) DETACH DELETE n;

// ---------- STEP 2: create 100 users ----------
UNWIND [
  {id: 1, name: 'Kavya Rana', course: 'B.Tech CSE', skills: ['Data_Structures', 'Operating_Systems', 'Problem_Solving', 'Communication'], interests: ['Hackathons', 'Research'], password: 'ehn#$QJ2ay'},
  {id: 2, name: 'Bhavya Bhandari', course: 'B.Tech CSE AI ML', skills: ['Deep_Learning', 'Kubernetes', 'Critical_Thinking'], interests: ['Artificial_Intelligence', 'Research', 'Robotics'], password: 'oKY!HDME72'},
  {id: 3, name: 'Gaurav Bhandari', course: 'MCA', skills: ['Android_Development', 'Swift', 'Time_Management'], interests: ['Gaming', 'Design'], password: 'ysxE&6QUxf'},
  {id: 4, name: 'Nikhil Saini', course: 'BA Journalism and Mass Communication', skills: ['Content_Writing', 'Video_Editing', 'Leadership', 'Teamwork'], interests: ['Music', 'Travel'], password: 'VU2*e%7Nf$'},
  {id: 5, name: 'Varun Chauhan', course: 'B.Sc Data Science', skills: ['SQL', 'Power_BI', 'Teamwork'], interests: ['Research', 'Robotics'], password: '?kFA7utwoT'},
  {id: 6, name: 'Vishal Kapoor', course: 'B.Des Graphic Design', skills: ['Graphic_Design', 'Social_Media', 'Communication', 'Time_Management'], interests: ['Reading', 'Sports'], password: 'd?fP*Wc#g8'},
  {id: 7, name: 'Juhi Mishra', course: 'B.Sc Computer Science', skills: ['C++', 'DBMS', 'Problem_Solving'], interests: ['Competitive_Programming', 'Hackathons'], password: 'ez$YenYT4q'},
  {id: 8, name: 'Radhika Sharma', course: 'B.Tech CSE', skills: ['Cloud_Computing', 'Critical_Thinking', 'Time_Management', 'Communication'], interests: ['Open_Source', 'Cloud_Technologies', 'Automation'], password: 'nD$CA#hM83'},
  {id: 9, name: 'Mohit Semwal', course: 'BCA', skills: ['Firebase', 'Android_Development', 'Communication', 'Time_Management'], interests: ['Startups', 'Design', 'Gaming'], password: '$z3iKw%QF$'},
  {id: 10, name: 'Saurabh Rawat', course: 'MCA', skills: ['Linux', 'Penetration_Testing', 'Critical_Thinking'], interests: ['Open_Source', 'Ethical_Hacking'], password: 'HHi8#dirr@'},
  {id: 11, name: 'Meera Bhatt', course: 'B.Tech CSE AI ML', skills: ['Deep_Learning', 'SQL', 'Communication', 'Teamwork'], interests: ['Robotics', 'Data_Analytics', 'Research'], password: 'F2WZ@DmF@y'},
  {id: 12, name: 'Aditya Joshi', course: 'B.Sc Data Science', skills: ['Statistics', 'Python', 'Communication', 'Critical_Thinking', 'Leadership'], interests: ['Robotics', 'Artificial_Intelligence'], password: 'ak#&hFDq7n'},
  {id: 13, name: 'Karan Kumar', course: 'B.Tech Electronics and Communication', skills: ['Cloud_Computing', 'Git', 'Time_Management'], interests: ['Cloud_Technologies', 'Automation', 'Startups'], password: 'h2DjM%7M4b'},
  {id: 14, name: 'Lavanya Rawat', course: 'BBA Marketing', skills: ['Public_Speaking', 'Social_Media', 'Critical_Thinking', 'Communication'], interests: ['Reading', 'Sports'], password: '?S%c4g!7J*'},
  {id: 15, name: 'Neha Bhandari', course: 'B.Tech CSE', skills: ['Firebase', 'Swift', 'Communication'], interests: ['Mobile_Apps', 'Startups'], password: 'HEGGe##b@8'},
  {id: 16, name: 'Esha Nautiyal', course: 'B.Tech CSE AI ML', skills: ['Data_Science', 'Python', 'Problem_Solving', 'Time_Management', 'Leadership'], interests: ['Research', 'Data_Analytics'], password: '47CSr9v!?5'},
  {id: 17, name: 'Rohan Joshi', course: 'B.Tech IT', skills: ['C', 'Data_Structures', 'Teamwork', 'Communication', 'Problem_Solving'], interests: ['Competitive_Programming', 'Gaming', 'Research'], password: 'F@&gQtis9W'},
  {id: 18, name: 'Jiya Verma', course: 'B.Sc Computer Science', skills: ['C++', 'DBMS', 'Problem_Solving'], interests: ['Competitive_Programming', 'Gaming'], password: 'i?*xPR89zr'},
  {id: 19, name: 'Palak Thakur', course: 'MCA', skills: ['Android_Development', 'Swift', 'Time_Management', 'Communication'], interests: ['Gaming', 'Startups', 'Design'], password: '7B@TEsmYY@'},
  {id: 20, name: 'Lokesh Bhandari', course: 'BBA Business Analytics', skills: ['Deep_Learning', 'SQL', 'Communication', 'Operating_Systems'], interests: ['Robotics', 'Artificial_Intelligence'], password: '*4Hg#b6Lin'},
  {id: 21, name: 'Siddharth Rawat', course: 'B.Sc Computer Science', skills: ['NodeJS', 'Django', 'Critical_Thinking', 'Leadership'], interests: ['Startups', 'Design'], password: 'cn65UX9Mz*'},
  {id: 22, name: 'Simran Thakur', course: 'B.Tech CSE', skills: ['Networking', 'Linux', 'Problem_Solving'], interests: ['Ethical_Hacking', 'Open_Source'], password: 'oU&zF&xy7S'},
  {id: 23, name: 'Ankit Panwar', course: 'B.Tech CSE', skills: ['Operating_Systems', 'DBMS', 'Time_Management', 'Teamwork', 'Problem_Solving'], interests: ['Hackathons', 'Competitive_Programming'], password: 'zk?6GwJWYP'},
  {id: 24, name: 'Tarun Semwal', course: 'B.Tech Cyber Security', skills: ['Linux', 'Teamwork', 'Communication', 'Leadership', 'Time_Management'], interests: ['Research', 'Ethical_Hacking'], password: 'MF&W*@5BLi'},
  {id: 25, name: 'Yash Negi', course: 'B.Tech IT', skills: ['C++', 'DBMS', 'Problem_Solving', 'Digital_Forensics'], interests: ['Competitive_Programming', 'Gaming', 'Research'], password: '!Tk8b@gtgU'},
  {id: 26, name: 'Sakshi Nautiyal', course: 'B.Com', skills: ['Social_Media', 'Content_Writing', 'Critical_Thinking', 'Leadership'], interests: ['Sports', 'Travel'], password: 'Gtt%VBHa3Z'},
  {id: 27, name: 'Pallavi Panwar', course: 'BCA', skills: ['Swift', 'Java', 'Leadership'], interests: ['Startups', 'Gaming'], password: 'xm!&3s&EvS'},
  {id: 28, name: 'Parth Rana', course: 'MCA', skills: ['Java', 'Problem_Solving', 'Critical_Thinking'], interests: ['Design', 'Gaming', 'Startups'], password: '7@Gn$UjHx4'},
  {id: 29, name: 'Khushi Bhatt', course: 'BA English Literature', skills: ['Video_Editing', 'Graphic_Design', 'Problem_Solving', 'Time_Management'], interests: ['Reading', 'Music'], password: 'S2n#tca8S%'},
  {id: 30, name: 'Ritika Yadav', course: 'BA Journalism and Mass Communication', skills: ['Social_Media', 'Public_Speaking', 'Leadership', 'Teamwork'], interests: ['Reading', 'Music'], password: 'adk6cRZ2S@'},
  {id: 31, name: 'Swati Verma', course: 'B.Des Graphic Design', skills: ['Digital_Marketing', 'Video_Editing', 'Leadership'], interests: ['Travel', 'Music'], password: 'mwD&LtZzV2'},
  {id: 32, name: 'Anjali Semwal', course: 'BBA Marketing', skills: ['Digital_Marketing', 'Public_Speaking', 'Critical_Thinking'], interests: ['Travel', 'Music'], password: '4GfCdwA%n$'},
  {id: 33, name: 'Raghav Mishra', course: 'M.Sc Data Science', skills: ['Python', 'Communication', 'Time_Management'], interests: ['Robotics', 'Research', 'Artificial_Intelligence'], password: 'PibhZ4!Bza'},
  {id: 34, name: 'Nisha Bisht', course: 'B.Tech Cyber Security', skills: ['Cyber_Security', 'Digital_Forensics', 'UI_UX_Design'], interests: ['Open_Source', 'Ethical_Hacking', 'Capture_The_Flag'], password: 'z*NP4R76@*'},
  {id: 35, name: 'Pooja Verma', course: 'B.Com', skills: ['Social_Media', 'Digital_Marketing', 'Content_Writing', 'Communication'], interests: ['Photography', 'Travel'], password: 'Yuz$pm7Ykw'},
  {id: 36, name: 'Komal Bhatt', course: 'MCA', skills: ['Linux', 'Penetration_Testing', 'Critical_Thinking'], interests: ['Open_Source', 'Research', 'Ethical_Hacking'], password: 'qZCTu?Z4s?'},
  {id: 37, name: 'Dev Uniyal', course: 'BA English Literature', skills: ['Content_Writing', 'Public_Speaking', 'Time_Management', 'Leadership'], interests: ['Music', 'Sports'], password: 'WJ%ru*n74*'},
  {id: 38, name: 'Chirag Verma', course: 'B.Sc Computer Science', skills: ['C', 'Operating_Systems', 'C++', 'Teamwork'], interests: ['Gaming', 'Research'], password: 'R7z%L!mrjU'},
  {id: 39, name: 'Naina Joshi', course: 'B.Tech Electronics and Communication', skills: ['Kubernetes', 'Git', 'Leadership', 'DBMS'], interests: ['Cloud_Technologies', 'Startups', 'Automation'], password: '5*wXrpZYrE'},
  {id: 40, name: 'Shubham Gupta', course: 'B.Tech Electronics and Communication', skills: ['Cloud_Computing', 'Git', 'Time_Management'], interests: ['Cloud_Technologies', 'Startups', 'Open_Source'], password: '@LweNCm9UF'},
  {id: 41, name: 'Sandeep Bisht', course: 'M.Sc Data Science', skills: ['SQL', 'Communication', 'Problem_Solving', 'Teamwork', 'Leadership'], interests: ['Data_Analytics', 'Research', 'Artificial_Intelligence'], password: 'EdjJ@4scbp'},
  {id: 42, name: 'Yamini Rawat', course: 'B.Tech IT', skills: ['NodeJS', 'JavaScript', 'Leadership', 'Operating_Systems'], interests: ['Open_Source', 'Freelancing'], password: 'ULsG3$XqPp'},
  {id: 43, name: 'Divya Verma', course: 'BA Journalism and Mass Communication', skills: ['Video_Editing', 'Social_Media', 'Public_Speaking', 'Leadership'], interests: ['Music', 'Photography'], password: 'o7&fS8Sehm'},
  {id: 44, name: 'Arjun Negi', course: 'M.Tech CSE', skills: ['Linux', 'Networking', 'Critical_Thinking'], interests: ['Capture_The_Flag', 'Ethical_Hacking'], password: '$@R6*aP$rs'},
  {id: 45, name: 'Aditi Joshi', course: 'MCA', skills: ['Swift', 'Java', 'Leadership', 'Kubernetes'], interests: ['Startups', 'Gaming'], password: 'E89CQV?3vh'},
  {id: 46, name: 'Diya Rana', course: 'B.Des Graphic Design', skills: ['Graphic_Design', 'Digital_Marketing', 'Teamwork', 'Problem_Solving'], interests: ['Travel', 'Photography'], password: 'Zkq!G99S9?'},
  {id: 47, name: 'Amit Singh', course: 'BBA Marketing', skills: ['Digital_Marketing', 'Public_Speaking', 'Leadership'], interests: ['Sports', 'Reading'], password: '6VX3hu8@ze'},
  {id: 48, name: 'Aarohi Rawat', course: 'MCA', skills: ['Cloud_Computing', 'Git', 'Time_Management'], interests: ['Cloud_Technologies', 'Automation'], password: '**6BLww6Hr'},
  {id: 49, name: 'Kartik Agarwal', course: 'B.Com', skills: ['Content_Writing', 'Public_Speaking', 'Social_Media', 'Critical_Thinking', 'Leadership'], interests: ['Travel', 'Sports'], password: '7efaDS4%Fj'},
  {id: 50, name: 'Himanshu Singh', course: 'BA English Literature', skills: ['Public_Speaking', 'Content_Writing', 'Communication', 'Leadership'], interests: ['Travel', 'Music'], password: '@9rhuQ%Rur'},
  {id: 51, name: 'Zoya Semwal', course: 'B.Tech IT', skills: ['Android_Development', 'Kotlin', 'Critical_Thinking', 'Time_Management'], interests: ['Design', 'Gaming', 'Startups'], password: 'L*6?ReMykT'},
  {id: 52, name: 'Garima Bhandari', course: 'B.Tech IT', skills: ['NodeJS', 'Django', 'Critical_Thinking'], interests: ['Startups', 'Open_Source', 'Design'], password: '!kWK$Nw5K*'},
  {id: 53, name: 'Vivaan Thakur', course: 'M.Tech CSE', skills: ['AWS', 'Problem_Solving', 'Communication', 'Time_Management'], interests: ['Automation', 'Cloud_Technologies'], password: '5C!8iDU&h8'},
  {id: 54, name: 'Trisha Verma', course: 'BBA Business Analytics', skills: ['Data_Science', 'DBMS', 'Problem_Solving', 'Time_Management', 'Critical_Thinking'], interests: ['Robotics', 'Artificial_Intelligence', 'Data_Analytics'], password: 'yPB7P&%Kbh'},
  {id: 55, name: 'Harsh Yadav', course: 'M.Tech CSE', skills: ['Linux', 'Networking', 'Critical_Thinking', 'Time_Management'], interests: ['Capture_The_Flag', 'Research'], password: 'i2%sh38#bQ'},
  {id: 56, name: 'Mahima Pandey', course: 'B.Tech Electronics and Communication', skills: ['Git', 'Cryptography', 'Communication'], interests: ['Startups', 'Open_Source', 'Cloud_Technologies'], password: 'Sp3KE9$Tx@'},
  {id: 57, name: 'Ayush Bhatt', course: 'B.Tech CSE', skills: ['Kubernetes', 'Git', 'Leadership', 'DBMS'], interests: ['Cloud_Technologies', 'Automation'], password: '4uH#3er$SY'},
  {id: 58, name: 'Riya Mishra', course: 'M.Sc Data Science', skills: ['Python', 'Data_Science', 'Critical_Thinking', 'Problem_Solving'], interests: ['Research', 'Artificial_Intelligence', 'Data_Analytics'], password: 'xsY8rA&rQP'},
  {id: 59, name: 'Yogesh Kapoor', course: 'BCA', skills: ['NodeJS', 'Django', 'Critical_Thinking', 'Communication'], interests: ['Startups', 'Design'], password: 'vkikQzEW2?'},
  {id: 60, name: 'Sahil Semwal', course: 'B.Sc Computer Science', skills: ['MongoDB', 'UI_UX_Design', 'Problem_Solving', 'Critical_Thinking'], interests: ['Freelancing', 'Open_Source'], password: 'w7YM#gftV8'},
  {id: 61, name: 'Mayank Mishra', course: 'B.Tech IT', skills: ['Operating_Systems', 'C', 'Leadership', 'Digital_Forensics'], interests: ['Research', 'Gaming', 'Competitive_Programming'], password: 'fMtD7H%Cc%'},
  {id: 62, name: 'Dhruv Kumar', course: 'B.Tech IT', skills: ['React', 'JavaScript', 'Leadership'], interests: ['Design', 'Freelancing'], password: 'jJF9VH@@pv'},
  {id: 63, name: 'Aarav Joshi', course: 'B.Sc Data Science', skills: ['Critical_Thinking', 'Teamwork', 'Problem_Solving', 'Leadership'], interests: ['Data_Analytics', 'Research'], password: 'Gx3TimB$Tr'},
  {id: 64, name: 'Lakshya Nautiyal', course: 'B.Sc Computer Science', skills: ['React', 'JavaScript', 'Leadership', 'Kubernetes'], interests: ['Design', 'Startups'], password: 'jr4#xff2TE'},
  {id: 65, name: 'Nidhi Saini', course: 'BCA', skills: ['Problem_Solving', 'Communication', 'Time_Management', 'Critical_Thinking'], interests: ['Startups', 'Gaming'], password: 'nvrCE3xw%&'},
  {id: 66, name: 'Ishaan Agarwal', course: 'B.Tech CSE', skills: ['Swift', 'Firebase', 'Teamwork', 'Time_Management', 'Communication'], interests: ['Mobile_Apps', 'Gaming'], password: '?up9Fa$!a3'},
  {id: 67, name: 'Vedant Pandey', course: 'B.Tech CSE', skills: ['Networking', 'Linux', 'Problem_Solving', 'Digital_Forensics'], interests: ['Ethical_Hacking', 'Research', 'Capture_The_Flag'], password: 'Lh&8S9%iva'},
  {id: 68, name: 'Akash Negi', course: 'B.Tech IT', skills: ['Kotlin', 'Swift', 'Critical_Thinking', 'Time_Management', 'Communication'], interests: ['Mobile_Apps', 'Gaming'], password: 'UdGGu*9ESV'},
  {id: 69, name: 'Shreya Joshi', course: 'MCA', skills: ['Kubernetes', 'Git', 'Leadership', 'Problem_Solving'], interests: ['Cloud_Technologies', 'Startups', 'Automation'], password: 'FL4eWtTsL#'},
  {id: 70, name: 'Ananya Pandey', course: 'BBA Business Analytics', skills: ['Python', 'Data_Science', 'Critical_Thinking'], interests: ['Research', 'Robotics'], password: 'ukNe&5W7?T'},
  {id: 71, name: 'Charu Singh', course: 'BCA', skills: ['Problem_Solving', 'Leadership', 'Teamwork'], interests: ['Startups', 'Freelancing'], password: '92mV**aVK4'},
  {id: 72, name: 'Priya Nautiyal', course: 'BCA', skills: ['Time_Management', 'Critical_Thinking', 'Communication', 'Leadership'], interests: ['Competitive_Programming', 'Gaming'], password: 'yR383?wH8h'},
  {id: 73, name: 'Utkarsh Agarwal', course: 'M.Tech CSE', skills: ['Linux', 'Penetration_Testing', 'Critical_Thinking'], interests: ['Open_Source', 'Capture_The_Flag', 'Research'], password: '$zu4C79WMN'},
  {id: 74, name: 'Urvashi Thakur', course: 'B.Sc Computer Science', skills: ['NodeJS', 'JavaScript', 'Leadership'], interests: ['Open_Source', 'Design'], password: 'dF$kv6o6j3'},
  {id: 75, name: 'Tushar Agarwal', course: 'B.Sc Data Science', skills: ['Power_BI', 'Machine_Learning', 'Communication', 'Leadership'], interests: ['Artificial_Intelligence', 'Robotics', 'Research'], password: '95?t6EuiHd'},
  {id: 76, name: 'Tanya Semwal', course: 'M.Tech CSE', skills: ['Problem_Solving', 'Communication', 'Teamwork', 'Leadership'], interests: ['Open_Source', 'Ethical_Hacking'], password: 'Ugk9h6%k2A'},
  {id: 77, name: 'Kritika Rawat', course: 'B.Tech CSE', skills: ['Kubernetes', 'DevOps', 'Cloud_Computing'], interests: ['Startups', 'Cloud_Technologies', 'Automation'], password: 'DFH65#boC%'},
  {id: 78, name: 'Heena Joshi', course: 'B.Sc Data Science', skills: ['SQL', 'Power_BI', 'Teamwork'], interests: ['Research', 'Artificial_Intelligence'], password: 'aN#K4!5vsQ'},
  {id: 79, name: 'Jatin Gupta', course: 'B.Tech CSE AI ML', skills: ['Python', 'Deep_Learning', 'Teamwork', 'Communication', 'Leadership'], interests: ['Research', 'Artificial_Intelligence'], password: 'tmTxWGk%k6'},
  {id: 80, name: 'Piyush Verma', course: 'B.Tech CSE', skills: ['C', 'Algorithms', 'Leadership'], interests: ['Competitive_Programming', 'Gaming'], password: 'e!enjj4B3k'},
  {id: 81, name: 'Naman Thakur', course: 'MCA', skills: ['Networking', 'Linux', 'Problem_Solving', 'Operating_Systems'], interests: ['Ethical_Hacking', 'Open_Source'], password: 'mgP7B$LHf6'},
  {id: 82, name: 'Abhishek Agarwal', course: 'B.Tech IT', skills: ['Operating_Systems', 'C', 'Leadership'], interests: ['Research', 'Gaming'], password: '%6qWer54*R'},
  {id: 83, name: 'Ira Sharma', course: 'B.Tech CSE', skills: ['Operating_Systems', 'C', 'Leadership'], interests: ['Research', 'Hackathons'], password: 'R2q!xTsRrh'},
  {id: 84, name: 'Rahul Agarwal', course: 'B.Tech IT', skills: ['NodeJS', 'JavaScript', 'Leadership'], interests: ['Open_Source', 'Freelancing'], password: 'XC9*Rp!?#3'},
  {id: 85, name: 'Vikas Mehta', course: 'BCA', skills: ['React', 'JavaScript', 'Leadership', 'Operating_Systems'], interests: ['Design', 'Open_Source'], password: 'qRjkqVu&4W'},
  {id: 86, name: 'Mansi Verma', course: 'B.Sc Computer Science', skills: ['Web_Development', 'JavaScript', 'Teamwork', 'Communication'], interests: ['Startups', 'Freelancing'], password: 'W&W865whz?'},
  {id: 87, name: 'Deepak Sharma', course: 'BCA', skills: ['Teamwork', 'Leadership', 'Problem_Solving', 'Communication'], interests: ['Gaming', 'Research'], password: 'P34YLxBjA#'},
  {id: 88, name: 'Pranav Yadav', course: 'B.Tech IT', skills: ['Leadership', 'Critical_Thinking', 'Teamwork', 'Time_Management'], interests: ['Design', 'Startups'], password: 'j9HRiJhM!J'},
  {id: 89, name: 'Muskan Mehta', course: 'B.Sc Computer Science', skills: ['Problem_Solving', 'Leadership', 'Time_Management', 'Critical_Thinking'], interests: ['Competitive_Programming', 'Research'], password: '3%aH7M@UHg'},
  {id: 90, name: 'Manish Gupta', course: 'MCA', skills: ['Leadership', 'Communication', 'Time_Management'], interests: ['Ethical_Hacking', 'Open_Source'], password: '$4@yp8tQR?'},
  {id: 91, name: 'Tanvi Thakur', course: 'B.Tech IT', skills: ['Swift', 'Java', 'Leadership'], interests: ['Startups', 'Mobile_Apps'], password: 'q$yiUE*6?p'},
  {id: 92, name: 'Sneha Sharma', course: 'M.Sc Data Science', skills: ['SQL', 'Power_BI', 'Teamwork'], interests: ['Research', 'Data_Analytics'], password: '36bxaNc3$J'},
  {id: 93, name: 'Rishabh Rana', course: 'B.Tech CSE AI ML', skills: ['Python', 'Data_Science', 'Critical_Thinking', 'Operating_Systems'], interests: ['Research', 'Robotics', 'Artificial_Intelligence'], password: '%7xu!9WZ#m'},
  {id: 94, name: 'Vaishnavi Singh', course: 'BCA', skills: ['Flutter', 'Time_Management', 'Communication'], interests: ['Mobile_Apps', 'Gaming'], password: 'XJF*jpN8dB'},
  {id: 95, name: 'Anshika Gupta', course: 'BCA', skills: ['Teamwork', 'Time_Management', 'Communication', 'Leadership'], interests: ['Gaming', 'Startups'], password: 'Y@B3%v9RSw'},
  {id: 96, name: 'Sanya Mehta', course: 'B.Tech CSE', skills: ['Android_Development', 'Swift', 'Time_Management', 'Kubernetes'], interests: ['Gaming', 'Startups', 'Design'], password: 'tdk@U5PeC8'},
  {id: 97, name: 'Krish Rana', course: 'M.Sc Data Science', skills: ['Data_Science', 'Power_BI', 'Communication', 'Time_Management'], interests: ['Research', 'Artificial_Intelligence', 'Robotics'], password: 'dE#J3#*baK'},
  {id: 98, name: 'Rashi Rana', course: 'M.Tech CSE', skills: ['AWS', 'Kubernetes', 'Communication'], interests: ['Startups', 'Automation'], password: 'wq4uPj$qJe'},
  {id: 99, name: 'Isha Bhandari', course: 'MCA', skills: ['Linux', 'Networking', 'Critical_Thinking', 'Kubernetes'], interests: ['Capture_The_Flag', 'Open_Source'], password: '62zGTQZ%iM'},
  {id: 100, name: 'Kunal Rana', course: 'B.Sc Data Science', skills: ['Deep_Learning', 'SQL', 'Communication'], interests: ['Robotics', 'Artificial_Intelligence'], password: '#Xv7!7Z3VW'}
] AS u
MERGE (n:User {id: u.id})
SET n.name = u.name, n.course = u.course, n.skills = u.skills,
    n.interests = u.interests, n.password = u.password;

// ---------- STEP 3: create 75 opportunities ----------
UNWIND [
  {id: 1, name: 'Python Programming Bootcamp', type: 'Course', skill: 'Python', interest: 'Artificial_Intelligence'},
  {id: 2, name: 'Python Automation Mini Project', type: 'Project', skill: 'Python', interest: 'Research'},
  {id: 3, name: 'Machine Learning Fundamentals', type: 'Course', skill: 'Machine_Learning', interest: 'Research'},
  {id: 4, name: 'Movie Recommendation System', type: 'Project', skill: 'Machine_Learning', interest: 'Artificial_Intelligence'},
  {id: 5, name: 'Deep Learning with Neural Networks', type: 'Course', skill: 'Deep_Learning', interest: 'Artificial_Intelligence'},
  {id: 6, name: 'Image Classification Workshop', type: 'Workshop', skill: 'Deep_Learning', interest: 'Research'},
  {id: 7, name: 'Data Science Essentials', type: 'Course', skill: 'Data_Science', interest: 'Research'},
  {id: 8, name: 'Student Performance Prediction Project', type: 'Project', skill: 'Data_Science', interest: 'Artificial_Intelligence'},
  {id: 9, name: 'Data Analysis with Pandas Workshop', type: 'Workshop', skill: 'Data_Analysis', interest: 'Artificial_Intelligence'},
  {id: 10, name: 'Sales Data Analysis Project', type: 'Project', skill: 'Data_Analysis', interest: 'Research'},
  {id: 11, name: 'SQL for Beginners', type: 'Course', skill: 'SQL', interest: 'Research'},
  {id: 12, name: 'Database Query Practice Workshop', type: 'Workshop', skill: 'SQL', interest: 'Artificial_Intelligence'},
  {id: 13, name: 'Statistics for Data Science', type: 'Course', skill: 'Statistics', interest: 'Artificial_Intelligence'},
  {id: 14, name: 'Probability and Sampling Workshop', type: 'Workshop', skill: 'Statistics', interest: 'Research'},
  {id: 15, name: 'Power BI Dashboard Workshop', type: 'Workshop', skill: 'Power_BI', interest: 'Research'},
  {id: 16, name: 'Business Dashboard Project', type: 'Project', skill: 'Power_BI', interest: 'Artificial_Intelligence'},
  {id: 17, name: 'HTML and CSS Crash Course', type: 'Course', skill: 'HTML_CSS', interest: 'Startups'},
  {id: 18, name: 'Responsive Landing Page Project', type: 'Project', skill: 'HTML_CSS', interest: 'Design'},
  {id: 19, name: 'JavaScript from Scratch', type: 'Course', skill: 'JavaScript', interest: 'Design'},
  {id: 20, name: 'Interactive Quiz App Project', type: 'Project', skill: 'JavaScript', interest: 'Startups'},
  {id: 21, name: 'React for Beginners', type: 'Course', skill: 'React', interest: 'Startups'},
  {id: 22, name: 'Weather App with React Project', type: 'Project', skill: 'React', interest: 'Design'},
  {id: 23, name: 'NodeJS Backend Workshop', type: 'Workshop', skill: 'NodeJS', interest: 'Design'},
  {id: 24, name: 'REST API Project', type: 'Project', skill: 'NodeJS', interest: 'Startups'},
  {id: 25, name: 'Full Stack Web Development Bootcamp', type: 'Course', skill: 'Web_Development', interest: 'Startups'},
  {id: 26, name: 'Personal Portfolio Website', type: 'Project', skill: 'Web_Development', interest: 'Design'},
  {id: 27, name: 'MongoDB Database Workshop', type: 'Workshop', skill: 'MongoDB', interest: 'Design'},
  {id: 28, name: 'Notes App with MongoDB Project', type: 'Project', skill: 'MongoDB', interest: 'Startups'},
  {id: 29, name: 'Django Web Framework Course', type: 'Course', skill: 'Django', interest: 'Startups'},
  {id: 30, name: 'Student Blog Website Project', type: 'Project', skill: 'Django', interest: 'Design'},
  {id: 31, name: 'UI UX Design Essentials', type: 'Course', skill: 'UI_UX_Design', interest: 'Design'},
  {id: 32, name: 'App Redesign Workshop', type: 'Workshop', skill: 'UI_UX_Design', interest: 'Startups'},
  {id: 33, name: 'Cyber Security Fundamentals', type: 'Course', skill: 'Cyber_Security', interest: 'Ethical_Hacking'},
  {id: 34, name: 'Capture The Flag Practice Workshop', type: 'Workshop', skill: 'Cyber_Security', interest: 'Open_Source'},
  {id: 35, name: 'Computer Networks Essentials', type: 'Course', skill: 'Networking', interest: 'Open_Source'},
  {id: 36, name: 'Network Setup Lab Workshop', type: 'Workshop', skill: 'Networking', interest: 'Ethical_Hacking'},
  {id: 37, name: 'Linux Command Line Workshop', type: 'Workshop', skill: 'Linux', interest: 'Ethical_Hacking'},
  {id: 38, name: 'Server Setup Project', type: 'Project', skill: 'Linux', interest: 'Open_Source'},
  {id: 39, name: 'Introduction to Cryptography', type: 'Course', skill: 'Cryptography', interest: 'Open_Source'},
  {id: 40, name: 'Password Strength Checker Project', type: 'Project', skill: 'Cryptography', interest: 'Ethical_Hacking'},
  {id: 41, name: 'Ethical Hacking Workshop', type: 'Workshop', skill: 'Penetration_Testing', interest: 'Ethical_Hacking'},
  {id: 42, name: 'Vulnerability Scanner Project', type: 'Project', skill: 'Penetration_Testing', interest: 'Open_Source'},
  {id: 43, name: 'Digital Forensics Basics', type: 'Course', skill: 'Digital_Forensics', interest: 'Open_Source'},
  {id: 44, name: 'Android App Development Course', type: 'Course', skill: 'Android_Development', interest: 'Design'},
  {id: 45, name: 'Expense Tracker Android App Project', type: 'Project', skill: 'Android_Development', interest: 'Gaming'},
  {id: 46, name: 'Kotlin Programming Workshop', type: 'Workshop', skill: 'Kotlin', interest: 'Gaming'},
  {id: 47, name: 'Simple 2D Game for Android Project', type: 'Project', skill: 'Kotlin', interest: 'Design'},
  {id: 48, name: 'Flutter Cross Platform Apps', type: 'Course', skill: 'Flutter', interest: 'Design'},
  {id: 49, name: 'Campus Event Reminder App Project', type: 'Project', skill: 'Flutter', interest: 'Gaming'},
  {id: 50, name: 'Java Programming Course', type: 'Course', skill: 'Java', interest: 'Gaming'},
  {id: 51, name: 'Library Management in Java Project', type: 'Project', skill: 'Java', interest: 'Design'},
  {id: 52, name: 'Swift Basics Workshop', type: 'Workshop', skill: 'Swift', interest: 'Design'},
  {id: 53, name: 'Firebase Backend Workshop', type: 'Workshop', skill: 'Firebase', interest: 'Mobile_Apps'},
  {id: 54, name: 'Chat App with Firebase Project', type: 'Project', skill: 'Firebase', interest: 'Startups'},
  {id: 55, name: 'Cloud Computing Fundamentals', type: 'Course', skill: 'Cloud_Computing', interest: 'Cloud_Technologies'},
  {id: 56, name: 'Deploy a Web App on the Cloud Project', type: 'Project', skill: 'Cloud_Computing', interest: 'Open_Source'},
  {id: 57, name: 'AWS Basics Workshop', type: 'Workshop', skill: 'AWS', interest: 'Open_Source'},
  {id: 58, name: 'Static Website Hosting Project', type: 'Project', skill: 'AWS', interest: 'Cloud_Technologies'},
  {id: 59, name: 'DevOps with CI CD Pipelines', type: 'Course', skill: 'DevOps', interest: 'Cloud_Technologies'},
  {id: 60, name: 'Automated Build Pipeline Project', type: 'Project', skill: 'DevOps', interest: 'Open_Source'},
  {id: 61, name: 'Docker and Containers Workshop', type: 'Workshop', skill: 'Docker', interest: 'Open_Source'},
  {id: 62, name: 'Containerize a Web App Project', type: 'Project', skill: 'Docker', interest: 'Cloud_Technologies'},
  {id: 63, name: 'Kubernetes for Beginners', type: 'Course', skill: 'Kubernetes', interest: 'Cloud_Technologies'},
  {id: 64, name: 'Git and GitHub Workshop', type: 'Workshop', skill: 'Git', interest: 'Startups'},
  {id: 65, name: 'Open Source Contribution Project', type: 'Project', skill: 'Git', interest: 'Automation'},
  {id: 66, name: 'C++ Programming Course', type: 'Course', skill: 'C++', interest: 'Hackathons'},
  {id: 67, name: 'Hackathon Preparation Workshop', type: 'Workshop', skill: 'C++', interest: 'Gaming'},
  {id: 68, name: 'C Programming Basics', type: 'Course', skill: 'C', interest: 'Gaming'},
  {id: 69, name: 'Mini Calculator Project in C', type: 'Project', skill: 'C', interest: 'Hackathons'},
  {id: 70, name: 'Data Structures and Algorithms in C++', type: 'Course', skill: 'Data_Structures', interest: 'Hackathons'},
  {id: 71, name: 'Library Management System Project', type: 'Project', skill: 'Data_Structures', interest: 'Gaming'},
  {id: 72, name: 'Competitive Programming Bootcamp', type: 'Course', skill: 'Algorithms', interest: 'Gaming'},
  {id: 73, name: 'Graph Based Route Finder Project', type: 'Project', skill: 'Algorithms', interest: 'Hackathons'},
  {id: 74, name: 'Operating Systems Concepts', type: 'Course', skill: 'Operating_Systems', interest: 'Hackathons'},
  {id: 75, name: 'DBMS Concepts Workshop', type: 'Workshop', skill: 'DBMS', interest: 'Research'}
] AS o
MERGE (n:Opportunity {id: o.id})
SET n.name = o.name, n.type = o.type, n.skill = o.skill, n.interest = o.interest;

// ---------- STEP 4: create friendships ----------
UNWIND [
  [1,7], [1,17], [1,23], [1,83], [2,5], [2,12], [2,20], [2,70], [2,93], [2,100], [3,9], [3,13], [3,27], [3,51], [3,66], [3,88], [4,31], [4,37], [5,11], [5,38], [5,41], [5,97], [5,100], [6,14], [6,24], [6,29], [6,53], [6,66], [6,94], [7,23], [7,37], [7,54], [7,57], [7,69], [8,40], [8,53], [9,15], [9,19], [9,96], [10,21], [10,22], [10,32], [10,55], [10,81], [10,99], [11,78], [11,92], [12,50], [12,58], [12,70], [12,93], [12,100], [13,51], [13,57], [13,77], [14,26], [14,35], [14,49], [14,50], [14,59], [15,19], [15,59], [15,66], [15,68], [15,91], [16,58], [16,83], [16,87], [16,90], [16,93], [16,97], [17,38], [17,46], [17,87], [18,23], [18,38], [18,53], [18,67], [18,76], [18,81], [19,27], [19,51], [19,66], [19,98], [20,86], [21,28], [21,62], [21,74], [21,84], [21,88], [22,36], [22,44], [22,55], [22,60], [22,73], [23,25], [23,29], [23,71], [24,37], [24,62], [24,83], [24,90], [24,95], [25,61], [25,67], [26,35], [27,28], [27,39], [27,71], [27,72], [28,45], [28,91], [29,40], [29,46], [29,53], [29,54], [30,32], [30,37], [30,43], [30,50], [30,57], [30,87], [31,32], [31,47], [32,47], [33,94], [33,97], [34,67], [34,81], [35,46], [35,50], [36,44], [36,55], [36,67], [36,81], [36,93], [37,49], [37,50], [38,82], [38,83], [38,93], [39,45], [39,91], [39,98], [40,56], [40,69], [41,78], [42,57], [42,83], [43,50], [44,67], [44,73], [45,96], [46,60], [46,71], [46,87], [51,59], [51,68], [51,96], [52,74], [52,84], [53,65], [53,98], [54,93], [55,67], [55,81], [55,89], [56,59], [56,69], [57,77], [57,98], [58,60], [59,65], [59,72], [61,80], [62,71], [62,74], [62,84], [62,88], [64,74], [64,84], [64,91], [64,96], [65,72], [66,94], [66,95], [69,71], [69,77], [69,98], [70,84], [70,97], [71,84], [72,89], [72,90], [72,95], [75,92], [75,97], [76,90], [77,98], [78,79], [78,100], [79,86], [79,100], [80,82], [80,83], [81,99], [86,95], [87,92], [87,95], [90,95], [90,100], [92,97]
] AS p
MATCH (a:User {id: p[0]}), (b:User {id: p[1]})
MERGE (a)-[:FRIEND]-(b);

// ---------- STEP 5: 20 users have already received a course ----------
UNWIND [
  [9,44], [11,5], [12,1], [17,68], [21,29], [38,66], [39,63], [40,55], [45,63], [48,55], [51,44], [54,7], [56,39], [58,7], [64,21], [75,3], [92,11], [93,74], [97,7], [98,63]
] AS t
MATCH (u:User {id: t[0]}), (o:Opportunity {id: t[1]})
MERGE (u)-[:TOOK]->(o);

// ---------- STEP 6: check totals ----------
MATCH (u:User) WITH count(u) AS users
MATCH (o:Opportunity) WITH users, count(o) AS opportunities
OPTIONAL MATCH ()-[r:FRIEND]-()
WITH users, opportunities, count(r) / 2 AS friendships
OPTIONAL MATCH ()-[t:TOOK]->()
RETURN users, opportunities, friendships, count(t) AS received_courses;

// ---------- STEP 7: how many users have how many friends ----------
MATCH (u:User)
OPTIONAL MATCH (u)-[:FRIEND]-(f:User)
WITH u, count(f) AS friends
RETURN friends, count(u) AS users
ORDER BY friends;
