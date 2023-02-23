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
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date, unsigned int month_date,  unsigned year_date, bool buy_sell_trans,  unsigned int number_shares, double trans_amount ){
  symbol=ticker_symbol;
  day=day_date;
  month=month_date;
  year=year_date;
  if(buy_sell_trans){
    trans_type = "Buy";
  }
  else{
    trans_type = "Sell";
  }
  shares=number_shares;
  amount=trans_amount;
  trans_id=assigned_trans_id;
  assigned_trans_id++;
  p_next=nullptr;
  acb_per_share=0;
  acb=0;
  share_balance=0;
  cgl=0;

}



// Destructor
// TASK 1
//
Transaction::~Transaction(){
  p_next=nullptr;
  delete p_next;

}


// Overloaded < operator.
// TASK 2
//
bool Transaction::operator< (Transaction const&other){
if(year < other.get_year() ){
  return true;
}
else if((year==other.get_year()) && (month < other.get_month())){
  return true;
}
else if ((year==other.get_year()) && (month == other.get_month()) && (day < other.get_day())){
  return true;
}
else if ((year==other.get_year()) && (month == other.get_month()) && (day == other.get_day()) && (trans_id>other.trans_id)){
  return true;
}
else{
  return false;
}
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


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History(){
  p_head=nullptr;

}

// Destructor
// TASK 3
//
History::~History(){
if(p_head !=nullptr){
Transaction *temp_current = p_head->get_next();
Transaction *temp_previous = p_head;
  while(temp_current!=nullptr){
    //temp_current=temp_current->get_next();
    delete temp_previous;
    temp_previous=temp_current;
    temp_current=temp_current->get_next();
  }
  delete temp_previous;
}
else{

}
}


// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){

  ece150::open_file();
  while (ece150::next_trans_entry()){
    Transaction* first_node = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
//      std::cout << "Inserting new Transaction Object"<< std::endl;
      //first_node->print();
    insert(first_node);
//      std::cout << "Finished Inserting new Transaction Object"<< std::endl;
  }
  ece150::close_file();

}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){
  //Node* new_node = new Node();
  if(get_p_head()==nullptr){
    p_head=p_new_trans;
  }else{
    Transaction *temp_trans {get_p_head()};
    while((temp_trans->get_next()) != nullptr){
      temp_trans = temp_trans->get_next(); 
//      std::cout << "Travel forward"<< std::endl;
    }
    temp_trans->set_next(p_new_trans);
  }
}



// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){
  //sets the new linked list
  Transaction *p_new_head = p_head;
  p_head = p_head->get_next();

  p_new_head->set_next(nullptr);

  // the new head of the p_head
  Transaction *traverse = p_head;

  while(p_head !=nullptr){
    // if the first value of the sorted list is greater than the p_head, then creates a temp variable to put in front 
    if (*p_head < *p_new_head) {
      Transaction * p_temp{p_head};
      p_head = p_head->get_next();

      p_temp->set_next(p_new_head);
      p_new_head=p_temp;

    } else {
      Transaction *temp_2 = p_head;
      p_head = p_head->get_next();

      Transaction *traverseSorted = p_new_head;
      
      // checks if the list is over and if the value of the node you are trying to insert is greater than the values of the linked list
      while((traverseSorted->get_next()  != nullptr) && (*(traverseSorted->get_next()) < (*temp_2))){    
        // goes through the list
        traverseSorted=(traverseSorted->get_next());
      }
      //once the condition is broken, inserts temp where appropriate
      temp_2->set_next(traverseSorted->get_next());
      traverseSorted->set_next(temp_2);
    }

    //increments the original linked list
  }
  
  //makes the the first value of the sorted list p new head
  p_head = p_new_head;

  //should I delete some stuff here?

  

}


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
  double acb{};
  double acbovershare{};
  double balance{};
  double cgl{};
  double previous{};

  Transaction *temp_update = p_head;
  while(temp_update !=nullptr){

    if(temp_update->get_trans_type())
    {

      acb +=temp_update->get_amount();
      temp_update ->set_acb(acb);

      balance += temp_update ->get_shares();
      temp_update ->set_share_balance(balance);

      acbovershare = (acb)/(double)(balance);
      temp_update -> set_acb_per_share(acbovershare);
      //previous=temp_update->get_acb_per_share();

    // temp_update = temp_update->get_next();
    }
    else{
      acb -=temp_update-> get_shares()*acbovershare;
      temp_update ->set_acb(acb);

      balance -= temp_update ->get_shares();
      temp_update -> set_share_balance(balance);
      //balance -= temp_update ->get_shares();

      acbovershare = (acb)/(double)(balance);
      temp_update -> set_acb_per_share(acbovershare);
      
      cgl= temp_update->get_amount() - temp_update-> get_shares()*acbovershare;
      temp_update->set_cgl(cgl);
    }

    temp_update = temp_update->get_next();

  }



}



// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year){
  double cgl{};

  Transaction *temp_compute = p_head;

  while(temp_compute != nullptr){
    
    if(temp_compute->get_year() == year){

      cgl+=temp_compute->get_cgl();

    }
    
    temp_compute = temp_compute->get_next();
  }

  return cgl;
  //return the capital gains at the specific year
}




// print() Print the transaction history.
//TASK 9
//
void History::print(){
  Transaction *temp_print = p_head;
  std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
  while(temp_print != nullptr){
    temp_print->print();
    temp_print = temp_print->get_next();
  }
  std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
}



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
