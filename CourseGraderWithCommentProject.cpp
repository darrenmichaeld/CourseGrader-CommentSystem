#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

const int MAX_RANKING_STARS =
    5; // at most a 5-star ranking in coments (from 1-star)
const int MAX_TITLE =
    100; // at most 100 characters (including the NULL character)

// A sorted linked list of StockItem, sorted by its id
struct Student {
  unsigned int sid;     // id is an unique identifier of the Student (e.g., 39)
  char name[MAX_TITLE]; // title is a description of the Course (e.g., History)
  unsigned int ranks_count; // The total number of star_ranks on existing
                               // courses the student did until now
  Student *next;               // The pointer pointing to the next Student
};

struct StarRank {
  unsigned int star; // The star-ranking the student gave to that course
  Student *student;  // The pointer showing to the struct of the student that
                     // made the star_rank
  StarRank *next;     // The pointer pointing to the StarRanks struct
};

// A sorted linked list represents a shopping cart, sorted by item->id
struct Course {
  unsigned int course_id; // course_id is an unique identifier of the Course
                          // (e.g., History)
  char name[MAX_TITLE];   // The course name
  int stars_count[MAX_RANKING_STARS]; // The count of stars from 1 (lowest) to
                                      // MAX_RANK (highest rank) of the course
  StarRank *star_rank_head; // The pointer pointing to the StarRanks struct
};

Student *create_student(const unsigned int sid, const char name[MAX_TITLE]) {
  Student *new_student = new Student;
  new_student->sid = sid;
  strcpy(new_student->name, name);
  new_student->ranks_count = 0;
  new_student->next = nullptr;
  return new_student;
}

Course *create_course(const unsigned int course_id,
                      const char name[MAX_TITLE]) {
  Course *new_course = new Course;
  new_course->course_id = course_id;
  strcpy(new_course->name, name);
  for (int i = 0; i < MAX_RANKING_STARS; i++) {
    new_course->stars_count[i] = 0;
  }
  new_course->star_rank_head = nullptr;
  return new_course;
}

// Given the number of courses, dynamicially creates and initializes the courses
// list array
Course **dynamic_init_course_array(const unsigned int num_courses) {
  Course **ret = nullptr;
  ret = new Course *[num_courses];
  for (int i = 0; i < num_courses; i++)
    ret[i] = nullptr;
  return ret;
}

// Helper function: search student and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_student(Student *head, const unsigned int sid, Student *&prev,
                    Student *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->sid == sid) {
      // found an existing entry
      return true;
    } else if (current->sid > sid) {
      // cannot find an existing entry
      return false;
    }
    prev = current;
  }
  return false;
}

// Helper function: search star_rank and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_star_rank(StarRank *head, const unsigned int sid, StarRank *&prev,
                    StarRank *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->student->sid == sid) {
      // found an existing entry
      return true;
    }
    prev = current;
  }
  return false;
}

// Helper function: search course and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_course(Course **&course_array, const unsigned int course_id,
                   const unsigned int num_courses, int &i) {
  Course *course;
  if (course_array != nullptr) {
    for (i = 0; i < num_courses; i++) {
      course = course_array[i];
      if (course == nullptr) {
        break;
      }

      if (course->course_id == course_id) {
        // found an existing entry
        return true;
      }
    }
  }
  return false;
}

bool add_course(Course **&course_array, const unsigned int course_id,
                const char name[MAX_TITLE], unsigned int &num_courses) {

  int coursepos = -1;
  int num_now = num_courses;
  if(search_course(course_array, course_id, num_now, coursepos))
  {
    return false;
  }

  int stop = 0;
  for(int i = 0; i < num_now;i++)
  {
    if(course_array[i]==nullptr && stop == 0)
    {
      course_array[i]=create_course(course_id, name);
      stop = stop + 1;
      return true;
    }
  }
  
  if(stop == 0)
  {
    int beforeaddnum = num_courses;
    num_courses = num_courses * 2;
    cout << "increase course array size to " << num_courses << endl;

    // Make a temp for place the array
    Course **tempcourse = dynamic_init_course_array(beforeaddnum);
    for(int i = 0; i < beforeaddnum; i++)
    {
      tempcourse[i]= course_array[i];
      course_array[i]= nullptr;
    }
    delete [] course_array;
    course_array = dynamic_init_course_array(num_courses);
    for(int i = 0; i < beforeaddnum; i++)
    {
      course_array[i]= tempcourse[i];
    }
    delete[] tempcourse;

    for(int i = 0; i < num_courses;i++)
    {
      if(course_array[i]==nullptr && stop == 0)
      {
        course_array[i]=create_course(course_id, name);
        stop = stop + 1;
        return true;
      }
    }
  }
  return false;
}

bool add_star_rank(Student *&student_head, unsigned int sid,
                 Course **&course_array, unsigned int course_id,
                 const unsigned int num_courses, int star) {
  Student *prev;
  Student *current;
  if(search_student(student_head, sid, prev, current)==0)
  {
    cout << "Failed to find student " << sid << " when add a star_rank." << endl;
    return false;
  }

  // Find course
  int ran = 0;
  if(search_course(course_array, course_id, num_courses, ran) == 0)
  {
    cout << "Failed to find course " << course_id << " when add a star_rank." << endl;
    return false;
  }

  // Find SID for a Course
  int stop = 0;
  for(int i = 0; i < num_courses; i++)
  {
    if(stop != 0)
        {
          break;
        }
        else if(course_array[i]->course_id == course_id)
        {
          stop += 1;

          if(course_array[i]->star_rank_head != nullptr)
          {
          int check = 0;
          for(StarRank *p = course_array[i]->star_rank_head; p!= nullptr; p = p->next)
          {
            if(p->student->sid == sid & check == 0)
            {
              check += 1;
              cout << "Failed to insert star_rank because the student " << sid
                   << " already have a star_rank." << endl;
                   return false;
            }
          
            }
          }
        }
  }
  
  int stopp = 0;
  // add star rank
  int position = 0;
  for(int i = 0; i < num_courses; i++)
  {
    if(stopp != 0)
    {
      break;
    }
    // find the same course id
    else if(course_array[i]->course_id == course_id)
    {
      stopp += 1;
      position = i;
      // add 1 point to the number of a star
      course_array[i]->stars_count[star-1] += 1;
      
      // make a list link for inputing the star_rank_head
      StarRank *newstarrank = new StarRank;
      newstarrank->star = star;
      newstarrank->next = nullptr;
        // using for to find the same sid
      for(Student *p = student_head; p!= nullptr; p = p->next)
      {
        if(p->sid == sid)
        {
          newstarrank->student = p;
          newstarrank->student->ranks_count += 1;
        }
      }
        // insert newstarrank to the star_rank_head
      if(course_array[position]->star_rank_head == nullptr)
      {
        newstarrank->next = nullptr;
        course_array[position]->star_rank_head = newstarrank;
        return true;
      }
      else if(course_array[position]->star_rank_head->next == nullptr)
      {
        course_array[position]->star_rank_head->next = newstarrank;
        return true;
      }
      else
      {
        StarRank* current = course_array[position]->star_rank_head;
        for(int position = 0; current->next != nullptr; current = current->next, ++position);
        newstarrank->next = current->next;
        current->next = newstarrank;

        delete current;
        current = nullptr;
        return true;
      }
      delete newstarrank;
      newstarrank = nullptr;
    }
  
  }
  return false;
}

bool add_student(Student *&student_head, const unsigned int sid,
                 const char name[MAX_TITLE]) 
{

  if(sid <= 0)
  {
    return false;
  }

  for(Student *check = student_head; check != nullptr; check = check->next)
    {
        if(check->sid == sid)
        {
          return false;
        }
    }
  
    Student* newstudent = create_student(sid, name);
    Student* prev = nullptr;
    Student* current = student_head;
    // insert at the beginning
    if(student_head == nullptr)
    {
        student_head = newstudent;
        return true;
    }
    else if(student_head->next == nullptr && student_head->sid > sid)
    {
      newstudent->next=student_head;
      student_head = newstudent;
      return true;
    }
    else if(student_head->next == nullptr && student_head->sid < sid)
    {
      student_head->next = newstudent;
      return true;
    }
    else{
    for(int position = 0; current->next != nullptr && current->next->sid < sid; current = current -> next, position++);
    newstudent->next = current->next;
    current->next = newstudent;
    return true;
    }
}

bool delete_star_rank(Student *&student_head, Course **&course_array,
                    const unsigned int sid, const unsigned int course_id,
                    const unsigned int num_courses) {
  int tes =0;
  if(search_course(course_array, course_id, num_courses, tes) == 0)
  {
    cout  << "Failed to delete star_rank, course " << course_id << " not found."
          << endl;
    return false;
  }
  int position = 0;
  int stop = 0;
  for(int i = 0; i < num_courses; i++)
  {
    if(course_array[i] != nullptr && course_array[i]->course_id == course_id && stop == 0)
    {
      position = i;
      stop += 1;
    }
  }
      StarRank *prevv;
      StarRank *currentt;
      if(search_star_rank(course_array[position]->star_rank_head, sid, prevv, currentt) == 0)
      {
        cout << "Failed to delete star_rank, star_rank not found in course " << course_id << endl;
        return false;
      }
      else if (course_array[position]!= nullptr)
      {
        stop += 1;
        int star = 0;
        StarRank *prev = nullptr;
        StarRank *current = course_array[position]->star_rank_head;
        // find course to be deleted

        while (current != nullptr && current->student->sid != sid)
        {
        prev = current;
        current = current->next;
        }

        star = current->star;

        if (current == course_array[position]->star_rank_head)
        {
        course_array[position]->stars_count[star-1] -= 1;
        current->student->ranks_count -= 1;
        StarRank* tempstar = course_array[position]->star_rank_head->next;
        delete course_array[position]->star_rank_head;
        course_array[position]->star_rank_head = tempstar;

        return true;
        }
        else
        {
        current->student->ranks_count -= 1;
        course_array[position]->stars_count[star-1] -= 1;
        prev->next = current->next;
        delete current;
        return true;

        }
      }
      delete prevv;
      prevv = nullptr;
      delete currentt;
      currentt = nullptr;
      return false;
    }
    
bool delete_course(Student *student_head, Course **&course_array,
                   const unsigned int course_id, unsigned int &num_courses) {

  int stop = 0;
  int tes = 0;
  if(search_course(course_array, course_id, num_courses, tes)==0)
  {
    cout << "Failed to delete course, course " << course_id << " not found." << endl;
    return false;
  }

  // Find how many array are not nullptr
  int fill = 0;
  for(int j = 0; j < num_courses; j++)
  {
    if(course_array[j]!= nullptr)
    {
      fill = j;
    }
  }
  int position = 0;
  for(int i = 0; i < num_courses; i++)
  {
    if(course_array[i] != nullptr && course_array[i]->course_id == course_id && stop == 0)
    {
      stop +=1;
      position = i;
      // base case
      if(position == 0)
      {
        StarRank *p = course_array[0]->star_rank_head;
        StarRank *temp = nullptr;
        for(; p!= nullptr; p = temp)
        {
          temp = p->next;
          int star = p->star;
          p->student->ranks_count -= 1;
          course_array[position]->stars_count[star-1] -= 1;
          delete p;

        }

        delete course_array[position];

        for(int j = position; j < fill; j++)
        {
          course_array[j] = course_array[j+1];
        }
        course_array[fill] = nullptr;
        
        if(course_array[(num_courses/2)] == nullptr && num_courses/2+1 != 2 )
        {
          num_courses /= 2;
          cout << "reduce course array size to " << num_courses << endl;

          Course **tempcourse = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            tempcourse[i]= course_array[i];
            course_array[i]= nullptr;
          }
          delete [] course_array;
          course_array = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            course_array[i]= tempcourse[i];
            tempcourse[i] = nullptr;
          }
          delete[] tempcourse;
        }

        return true;
      }
      
      //
      else if(position<fill && position != 0)
      {
        StarRank *p = course_array[position]->star_rank_head;
        StarRank *temp = nullptr;
        for(; p!= nullptr; p = temp)
          {
            temp = p->next;
            int star = p->star;
            p->student->ranks_count -= 1;
            course_array[position]->stars_count[star-1] -= 1;
            delete p;
          }

        delete course_array[position];

        for(int j = position; j < fill; j++)
        {
          course_array[j] = course_array[j+1];
          if(j == fill-1)
          {
            course_array[fill] = nullptr;
          }
        }
        
        if(course_array[(num_courses/2)] == nullptr && num_courses/2+1 != 2 )
        {
          num_courses /= 2;
          cout << "reduce course array size to " << num_courses << endl;

          Course **tempcourse = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            tempcourse[i]= course_array[i];
            course_array[i]= nullptr;
          }
          delete [] course_array;
          course_array = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            course_array[i]= tempcourse[i];
            tempcourse[i] = nullptr;
          }
          delete[] tempcourse;
        }
        return true;
      }
      // Last array but not last numcourse
      else if(position == fill)
      {
        StarRank *p = course_array[position]->star_rank_head;
        StarRank *temp = nullptr;
        for(; p!= nullptr; p = temp)
          {
            temp = p->next;
            int star = p->star;
            p->student->ranks_count -= 1;
            course_array[position]->stars_count[star-1] -= 1;
            delete p;
          }
          delete course_array[position];

          course_array[position] = nullptr;
        
        if(course_array[(num_courses/2)] == nullptr && num_courses/2+1 != 2 )
        {
          num_courses /= 2;
          cout << "reduce course array size to " << num_courses << endl;

          Course **tempcourse = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            tempcourse[i]= course_array[i];
            course_array[i]= nullptr;
          }
          delete [] course_array;
          course_array = dynamic_init_course_array(num_courses);
          for(int i = 0; i < num_courses; i++)
          {
            course_array[i]= tempcourse[i];
            tempcourse[i] = nullptr;
          }
          delete[] tempcourse;
        }
        return true;
      }
    }
  }
  return false;
}

void clean_up(Student *&student_head, StarRank *&star_rank_head,
              Course **&course_array, unsigned int &num_courses) {

  while (course_array[0] != nullptr) {
    delete_course(student_head, course_array, course_array[0]->course_id,
                  num_courses);
  }

  if (student_head != nullptr) {
    Student *student;
    while (student_head->next != nullptr) {
      student = student_head->next;
      student_head->next = student_head->next->next;
      delete student;
    }
    delete student_head;
    student_head = nullptr;
  }

  delete[] course_array; // delete the dynamically allocated 2D array
  course_array = nullptr;
}

void display_students(Student *student_head) {
  cout << "=== Student List ([sid, name, star_rank count]) ===" << endl;
  
  if(student_head != nullptr)
  {
    for(Student* p = student_head; p!=nullptr; p = p->next)
    {
        if(p->next != nullptr)
        cout << "[" << p->sid << ", "<< p->name << ", " << p->ranks_count << "] -> ";
        else if(p->next == nullptr)
        cout << "[" << p->sid << ", "<< p->name << ", " << p->ranks_count << "]";
    }
    cout << endl;
  }
  else
  {
  cout << "No items in the Student list" << endl; // Use this when no student exists
  }
}

void display_star_ranks(Course **course_array, const unsigned int num_courses,
                        const unsigned int course_id) {
  // If the course does not exist use the following cout
  int checkSid = 0;
  int coursePosition = 0;
  
  for(int i = 0; i < num_courses; i++)
  {
    if(checkSid != 0)
    {
      break;
    }
    else if(course_array[i] != nullptr)
    {
      if(course_array[i]->course_id == course_id)
      {
        checkSid += 1;
        coursePosition = i;
      }
    }
  }
  // No data in the array
  if(checkSid == 0)
  {
    cout << "Course not found " << endl; // Display this if course not found
  }
  else if(course_array[coursePosition]->stars_count[0] == 0 && course_array[coursePosition]->stars_count[1] == 0 && course_array[coursePosition]->stars_count[2] == 0 && course_array[coursePosition]->stars_count[3] == 0 && course_array[coursePosition]->stars_count[4] == 0)
  {
    cout << "star_ranks in course " << course_array[coursePosition]->name << " : No StarRanks in the course " << course_array[coursePosition]->name << endl; // __COURSE_NAME represents the name of the course
  }
  else
  {
    int sign = 0;
    cout << "star_ranks in course " << course_array[coursePosition]->name << " : ";
    for(StarRank *p = course_array[coursePosition]->star_rank_head; p != nullptr; p = p->next)
    {
      if(sign == 0)
      {
        cout << "[" << p->student->sid << ": " << p->star << "]" ;
      }
      else
      {
        cout << " -> [" << p->student->sid << ": " << p->star << "]" ;
      }
      sign += 1;
    }
    cout << endl;
  }  
}

void display_courses(Course **course_array, const unsigned int num_courses) {
  if(course_array[0] != nullptr)
  {
    for(int i = 0 ; i < num_courses; i++)
    {
      if(course_array[i] != nullptr)
      {
        cout << "course_id : " << course_array[i]->course_id << ", ";
        cout << "name : " << course_array[i]->name << ", ";
        cout << "stars_count :" << endl;
        cout << "*     " << course_array[i]->stars_count[0] << endl;
        cout << "**    " << course_array[i]->stars_count[1] << endl;
        cout << "***   " << course_array[i]->stars_count[2] << endl;
        cout << "****  " << course_array[i]->stars_count[3] << endl;
        cout << "***** " << course_array[i]->stars_count[4] << endl;
      }
    }
  }
  else
  {
  cout << "No course in the list " << endl;
  }
}

int main() {
  enum MeunOption {
    OPTION_DISPLAY_STUDENT_LIST = 0,
    OPTION_DISPLAY_COURSES,
    OPTION_DISPLAY_COMMENTS,
    // OPTION_DISPLAY_CURRENT_LIST,
    OPTION_INSERT_STUDENT,
    OPTION_INSERT_COURSE,
    OPTION_INSERT_COMMENT,
    OPTION_DELETE_COMMENT,
    OPTION_DELETE_COURSE,
    OPTION_EXIT_SYSTEM,
    MAX_MENU_OPTIONS
  };
  const int MAX_MENU_OPTIONS_LENGTH = 80;
  char menu_options[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
      "Display student list",
      "Display courses",
      "Display star_ranks of course ",
      "Insert a new student to the student list",
      "Insert a new course to the course list",
      "Insert a new star_rank to the star_rank list",
      "Delete a star_rank by student id and course id",
      "Delete a course by course id",
      "Exit the system"};

  Student *student_head = nullptr;
  StarRank *star_rank_head = nullptr;
  Course **course_array = nullptr;
  unsigned int num_courses = 0;
  int i, option;
  unsigned int sid, course_id, star;
  char name[MAX_TITLE] = "";
  bool ret = false;

  num_courses = 2;
  course_array = dynamic_init_course_array(num_courses);

  cout << "=== Simplified USTSPAC System ===" << endl;
  while (true) {
    cout << "=== Menu ===" << endl;
    for (i = 0; i < MAX_MENU_OPTIONS; i++)
      cout << i + 1 << ": " << menu_options[i]
           << endl; // shift by +1 when display

    cout << "Enter your option: " << endl;
    cin >> option;
    option = option - 1; // shift by -1 after entering the option

    // The invalid menu option handling
    if (option < 0 || option >= MAX_MENU_OPTIONS) {
      cout << "Invalid option" << endl;
      continue;
    }

    // Exit operations handling
    if (option == OPTION_EXIT_SYSTEM) {
      clean_up(student_head, star_rank_head, course_array, num_courses);
      break; // break the while loop
    }

    switch (option) {
    case OPTION_DISPLAY_STUDENT_LIST:
      display_students(student_head);
      break;
    case OPTION_DISPLAY_COURSES:
      display_courses(course_array, num_courses);
      break;
    case OPTION_DISPLAY_COMMENTS:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      display_star_ranks(course_array, num_courses, course_id);

      break;
    case OPTION_INSERT_STUDENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      cout << "Enter a name: " << endl;
      cin >> name;

      ret = add_student(student_head, sid, name);
      if (ret == false) {
        cout << "Failed to insert student " << sid << endl;
      } else {
        cout << sid << " is successfully inserted" << endl;
      }
      break;
    case OPTION_INSERT_COURSE:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      cout << "Enter a name: " << endl;
      cin >> name;

      ret = add_course(course_array, course_id, name, num_courses);
      if (ret == false) {
        cout << "Failed to insert course " << course_id << endl;
      } else {
        cout << course_id << " is successfully inserted" << endl;
      }
      break;
    case OPTION_INSERT_COMMENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      star = 0;
      cout << "Enter the star rank of this course: " << endl;
      cin >> star;
      if (star <= 0 || star > MAX_RANKING_STARS) {
        cout << "Enter a valid course id > 0 && < " << MAX_RANKING_STARS
             << endl;
        break;
      }

      ret = add_star_rank(student_head, sid, course_array, course_id, num_courses,
                        star);
      if (ret == false) {
        cout << "Failed to insert star_rank, sid: " << sid
             << ", course id: " << course_id << endl;
      } else {
        cout << " star_rank is successfully inserted" << endl;
      }
      break;
    case OPTION_DELETE_COMMENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }

      ret = delete_star_rank(student_head, course_array, sid, course_id,
                           num_courses);
      if (ret == false) {
        cout << "Failed to delete star_rank, sid: " << sid
             << ", course id: " << course_id << endl;
      } else {
        cout << " star_rank is successfully deleted" << endl;
      }
      break;
    case OPTION_DELETE_COURSE:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }

      ret = delete_course(student_head, course_array, course_id, num_courses);
      if (ret == false) {
        cout << "Failed to delete course, course id: " << course_id << endl;
      } else {
        cout << "course is successfully deleted" << endl;
      }
      break;
    default:
      break;
    } // end of switch (option)
  }

  return 0;
}