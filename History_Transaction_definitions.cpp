#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double
trans_amount)
{

  symbol = ticker_symbol;
  day = day_date;
  month =  month_date;
  year = year_date;
  
  
  if (buy_sell_trans == true) {
    trans_type = "Buy";
  } else {
    trans_type = "Sell";
  }

  shares = number_shares;
  amount = trans_amount;

  trans_id = assigned_trans_id;
  assigned_trans_id++;

  acb = 0.0;
  acb_per_share = 0.0;
  share_balance = 0;
  cgl = 0.0;

  p_next = nullptr;

}


// Destructor
Transaction::~Transaction(){}

// Overloaded < operator.
bool Transaction::operator<( Transaction const &other ){

if (year < other.year){
  return true;
}else if (year == other.year){
  if (month < other.month){
    return true;
  } else if (month == other.month){
    if (day < other.day){
      return true;
    } else if (day == other.day){
      if (trans_id < other.trans_id){
        return true;
      }
    }
  }
}

return false;

}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}

//Definitions for History class
////////////////////////////////////////////////////////////////////////////////
History::History(){
//initializes linked list to be empty
p_head = nullptr;
}

History::~History(){

Transaction *p_current {p_head};

while (p_current != nullptr){
  //while the list isn't empty, next pointer is what current is pointing to
  Transaction *p_next = p_current->get_next();
  delete p_current;
  p_current = p_next;
}

p_head = nullptr;

}


void History::insert(Transaction *p_new_trans){

Transaction *p_current {p_head};

//if the list is empty, assign the new node to the head
if (p_head == nullptr){
  p_head = p_new_trans;

}  else { //iterate througbh the list to find the last node
    while (p_current->get_next() != nullptr){
    //while the next node is not null pointer, move current node to the next one
      p_current = p_current->get_next();
      }
    //once while loop is exited, the temporary variable points to the lost node
    //now we can assigned the last node to point to the new one!
    p_current->set_next(p_new_trans);
  }

}


void History::read_history(){

ece150::open_file();

//read transactions from file until end of file
while (ece150::next_trans_entry()){

//initialize transaction with read data

std::string ticker_symbol {ece150::get_trans_symbol()};
unsigned int day_date {ece150::get_trans_day()};
unsigned int month_date {ece150::get_trans_month()};
unsigned year_date {ece150::get_trans_year()}; 
bool buy_sell_trans {ece150::get_trans_type()};
unsigned int number_shares {ece150::get_trans_shares()};
double trans_amount {ece150::get_trans_amount()};

//create new transaction and insert it
Transaction *p_new_transaction {new Transaction (ticker_symbol, day_date, month_date,
year_date, buy_sell_trans, number_shares, trans_amount)};
  
insert (p_new_transaction);
}

ece150::close_file();

}

void History::print(){

Transaction *p_current {p_head};

std::cout << "========== BEGIN TRANSACTION HISTORY ============"<< std::endl;

    while (p_current != nullptr){
  //while the next node is not null pointer, print it
    p_current->print();
    //now get next transaction
    p_current = p_current->get_next();
    }

  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;

}
	

void History::sort_by_date(){

if (p_head==nullptr){
  return; // shes empty
} else if (p_head->get_next() == nullptr){
  return; //one object
} 

Transaction *p_current {p_head}; //used to traverse the list
Transaction *p_smallest {nullptr}; //stores smallest value in unsorted linked list
Transaction *p_prev_smallest {nullptr}; //stores previous pointer to smallest value
Transaction *p_fake_new {nullptr}; //makes fake new linked list

//while the unsorted list IS NOT empty:
while (p_head !=nullptr ){

p_current = p_head;
p_smallest = p_current;
p_prev_smallest = nullptr;

//1. find smallest and its previous
  while (p_current->get_next() !=nullptr){ //ends if we have traversed entire list!
    if (*p_current->get_next() < *p_smallest){ //if the next one is smaller than the first, update smallest
        p_smallest = p_current->get_next();
        p_prev_smallest = p_current;
       } 
    p_current = p_current->get_next();
  }// now we have found the smallest one

//2. detach the smallest from the unsorted linked list
if (p_prev_smallest == nullptr){
  //first item is the smallest, connect p_head to next one (pop front)
   p_head = p_smallest->get_next();
} else {
  //smaller item was found, cut it out 
  p_prev_smallest->set_next(p_smallest->get_next());
}

//3. create new linked list
if (p_fake_new == nullptr){ //if the list hasn't started yet..
p_fake_new = p_smallest; // add the smallest node to the list

} else { 
  //if this another iteration, find the end of the new linked list and add the pointer there

Transaction *p_current_2{p_fake_new};
while (p_current_2->get_next() !=nullptr){ //ends if we have traversed entire list!
    p_current_2 = p_current_2->get_next();
  } 
//when this while loop exists, current pointer is at the end of the linked list
p_current_2->set_next(p_smallest);
}

//now, pointer has been added
//remove the extra data that the smallest pointer points to
p_smallest->set_next(nullptr);
} 

//once whle loop is exited, connect new list
p_head = p_fake_new;

}


void History::update_acb_cgl(){

if (p_head == nullptr){
  return; //empty list
}

//calculate values for the first one using p_head
p_head->set_acb(p_head->get_amount());
p_head->set_share_balance(p_head->get_shares());
p_head->set_acb_per_share(p_head->get_acb()/p_head->get_share_balance());
p_head->set_cgl(p_head->get_cgl());


//while there is a second or more transaction, update these values

Transaction *p_current {p_head};

while (p_current->get_next() != nullptr){

double up_acb {p_current->get_acb()};
unsigned int up_sb {p_current->get_share_balance()};
double up_acb_ps {p_current->get_acb_per_share()};
double up_cgl {0.0};

p_current = p_current->get_next();

//if its a buy:
if (p_current->get_trans_type()==true){
  p_current->set_acb(p_current->get_amount() + up_acb);
  p_current->set_share_balance(p_current->get_shares() + up_sb);
  p_current->set_acb_per_share(p_current->get_acb()/p_current->get_share_balance());
  p_current->set_cgl(0.0);
  } else {
  //if its a sell
    p_current->set_acb(up_acb - up_acb_ps*p_current->get_shares());
    p_current->set_share_balance(up_sb - p_current->get_shares());
    p_current->set_acb_per_share(up_acb_ps);
    p_current->set_cgl(p_current->get_amount() - p_current->get_shares()*up_acb_ps);
  }
}

}



double History::compute_cgl(unsigned int year){
  
double cgl {0.0};

Transaction *p_current {p_head};

while (p_current != nullptr){
    if (p_current->get_year() == year){
    cgl += p_current->get_cgl();
    }
    p_current = p_current->get_next();
}

return cgl;
}



////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TODO

// Destructor
// TODO

// insert(...): Insert transaction into linked list.
// TODO

// read_history(...): Read the transaction history from file.
// TODO

// print() Print the transaction history.
// TODO

// sort_by_date(): Sort the linked list by trade date.
// TODO

// update_acb_cgl(): Updates the ACB and CGL values.
// TODO

// compute_cgl(): )Compute the ACB, and CGL.
// TODO


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
