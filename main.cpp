#include<iostream>
#include<fstream>
#include<string>
#include<climits>
#include<sstream>
using namespace std;

struct Ride //Create a Ride data structure
{
	int clr=1;
	int rideNumber, rideCost, rideDuration, heapIndex = -1;
	Ride *parent, *lchild, *rchild;
};


bool comp(Ride *a, Ride *b){
	if(a->rideCost == b->rideCost ){
		return a->rideDuration>b->rideDuration;
	}
	return a->rideCost>b->rideCost;
}

typedef Ride *RidePtr;

class MinHeap      //Create a minheap datastructure
{
	
	RidePtr *heap_arr; 
	int max_size=5000;
	int hp_size;
public:
	MinHeap(){
        hp_size = 0;
	    heap_arr = new RidePtr[max_size];
    }
	void minHeapify(int x){   //Heapify function
        int l = lchild(x);
        int r = rchild(x);
        int least = x;
        if (l < hp_size && comp(heap_arr[x],heap_arr[l]))
            least = l;
        if (r < hp_size &&  comp(heap_arr[least],heap_arr[r]))
            least = r;
        if (least != x)
        {
            heap_arr[x]->heapIndex = least;
            heap_arr[least]->heapIndex = x;
            RidePtr tmp= heap_arr[x];
            heap_arr[x] = heap_arr[least];
            heap_arr[least] = tmp;
            minHeapify(least);
        }
    }
	int parent(int x) { return (x-1)/2; }
	int lchild(int x) { return (2*x + 1); }
	int rchild(int x) { return (2*x + 2); }
	RidePtr extractMin(){                     // extract min element in the min heap
        if (hp_size <= 0)
            return NULL;
        if (hp_size == 1)
        {
            hp_size--;
            return heap_arr[0];
        }
        RidePtr root = heap_arr[0];
        heap_arr[0] = heap_arr[hp_size-1];
        heap_arr[0]->heapIndex = 0;
        hp_size--;
        minHeapify(0);

        return root;
    }
	void reduceKey(int x, int new_val){
        heap_arr[x]->rideCost = new_val;
        while (x != 0 && comp(heap_arr[parent(x)],heap_arr[x]))
        {
            heap_arr[x]->heapIndex = parent(x);
            heap_arr[parent(x)]->heapIndex = x;
            RidePtr tmp= heap_arr[x];
            heap_arr[x] = heap_arr[parent(x)];
            heap_arr[parent(x)] = tmp;
        x = parent(x);
        }
    }
	RidePtr getMin() { return heap_arr[0]; }
	void delKey(int x){
        reduceKey(x, INT_MIN);
        extractMin();
    }
	void insertKey(RidePtr m){  //Insert elements in min heap
        if (hp_size == max_size)
        {
            cout << "\nOverflow: Could not insertKey\n";
            return;
        }
        hp_size++;
        int x = hp_size - 1;
        heap_arr[x] = m;
        m->heapIndex = x;
        while (x != 0 && comp(heap_arr[parent(x)],heap_arr[x]))
        {
            heap_arr[x]->heapIndex = parent(x);
            heap_arr[parent(x)]->heapIndex = x;
            RidePtr tmp= heap_arr[x];
            heap_arr[x] = heap_arr[parent(x)];
            heap_arr[parent(x)] = tmp;
        x = parent(x);
        }
    }
};

class RBTree{        //Create a RBT datastructure
    RidePtr root;
	RidePtr TNULL;
    public:
    void printRide(RidePtr r){
        cout<<"("<<r->rideNumber<<","<<r->rideCost<<","<<r->rideDuration<<"),";
    }
	RidePtr searchHelper(RidePtr ride, int key) { // Helper function to search for elements in rbt
		if (ride == TNULL || key == ride->rideNumber) {
			return ride;
		}

		if (key < ride->rideNumber) {
			return searchHelper(ride->lchild, key);
		} 
		return searchHelper(ride->rchild, key);
	}
	void initializeNULLRide(RidePtr ride, RidePtr parent) {
		ride->parent = parent;
		ride->lchild = nullptr;
		ride->rchild = nullptr;
		ride->clr = 0;
	}
	void rbtTransplant(RidePtr n, RidePtr o){ //Transplanting the tree
		if (n->parent == nullptr) {
			root = o;
		} else if (n == n->parent->lchild){
			n->parent->lchild = o;
		} else {
			n->parent->rchild = o;
		}
		o->parent = n->parent;
	}
	void fixingDelete(RidePtr a) { //Fixing the tree after deleting a ride
		RidePtr s;
		while (a != root && a->clr == 0) {
			if (a == a->parent->lchild) {
				s = a->parent->rchild;
				if (s->clr == 1) {
					s->clr = 0;
					a->parent->clr = 1;
					rotateLeft(a->parent);
					s = a->parent->rchild;
				}

				if (s->lchild->clr == 0 && s->rchild->clr == 0) {
					s->clr = 1;
					a = a->parent;
				} else {
					if (s->rchild->clr == 0) {
						s->lchild->clr = 0;
						s->clr = 1;
						rotateRight(s);
						s = a->parent->rchild;
					} 
					s->clr = a->parent->clr;
					a->parent->clr = 0;
					s->rchild->clr = 0;
					rotateLeft(a->parent);
					a = root;
				}
			} else {
				s = a->parent->lchild;
				if (s->clr == 1) {
					s->clr = 0;
					a->parent->clr = 1;
					rotateRight(a->parent);
					s = a->parent->lchild;
				}

				if (s->rchild->clr == 0 && s->rchild->clr == 0) {
					s->clr = 1;
					a = a->parent;
				} else {
					if (s->lchild->clr == 0) {
						s->rchild->clr = 0;
						s->clr = 1;
						rotateLeft(s);
						s = a->parent->lchild;
					} 
					s->clr = a->parent->clr;
					a->parent->clr = 0;
					s->lchild->clr = 0;
					rotateRight(a->parent);
					a = root;
				}
			} 
		}
		a->clr = 0;
	}
	void updateInsert(RidePtr m){  //Fixing the tree after insert
		RidePtr n;
		while (m->parent->clr == 1) {
			if (m->parent == m->parent->parent->rchild) {
				n = m->parent->parent->lchild;
				if (n->clr == 1) {
					n->clr = 0;
					m->parent->clr = 0;
					m->parent->parent->clr = 1;
					m = m->parent->parent;
				} else {
					if (m == m->parent->lchild) {
						m = m->parent;
						rotateRight(m);
					}
					m->parent->clr = 0;
					m->parent->parent->clr = 1;
					rotateLeft(m->parent->parent);
				}
			} else {
				n = m->parent->parent->rchild;

				if (n->clr == 1) {
					n->clr = 0;
					m->parent->clr = 0;
					m->parent->parent->clr = 1;
					m = m->parent->parent;	
				} else {
					if (m == m->parent->rchild) {
						m = m->parent;
						rotateLeft(m);
					}
					m->parent->clr = 0;
					m->parent->parent->clr = 1;
					rotateRight(m->parent->parent);
				}
			}
			if (m == root) {
				break;
			}
		}
		root->clr = 0;
	}
	void rideDeleteHelper(RidePtr ride, int key) {  //Helper function to delete a ride
		RidePtr c = TNULL;
		RidePtr a, b;
		while (ride != TNULL){
			if (ride->rideNumber == key) {
				c = ride;
			}

			if (ride->rideNumber <= key) {
				ride = ride->rchild;
			} else {
				ride = ride->lchild;
			}
		}

		if (c == TNULL) {
			return;
		}  

		b = c;
		int y_original_clr = b->clr;
		if (c->lchild == TNULL) {
			a = c->rchild;
			rbtTransplant(c, c->rchild);
		} else if (c->rchild == TNULL) {
			a = c->lchild;
			rbtTransplant(c, c->lchild);
		} else {
			b = min(c->rchild);
			y_original_clr = b->clr;
			a = b->rchild;
			if (b->parent == c) {
				a->parent = b;
			} else {
				rbtTransplant(b, b->rchild);
				b->rchild = c->rchild;
				b->rchild->parent = b;
			}

			rbtTransplant(c, b);
			b->lchild = c->lchild;
			b->lchild->parent = b;
			b->clr = c->clr;
		}
		delete c;
		if (y_original_clr == 0){
			fixingDelete(a);
		}
	}
	RBTree() {
		TNULL = new Ride;
		TNULL->clr = 0;
		TNULL->lchild = nullptr;
		TNULL->rchild = nullptr;
		root = TNULL;
	}

	RidePtr search(int m) {
		return searchHelper(this->root, m);
	}

	RidePtr min(RidePtr ride) {
		while (ride->lchild != TNULL) {
			ride = ride->lchild;
		}
		return ride;
	}

	void rotateRight(RidePtr a) {  //rotation function
		RidePtr b = a->lchild;
		a->lchild = b->rchild;
		if (b->rchild != TNULL) {
			b->rchild->parent = a;
		}
		b->parent = a->parent;
		if (a->parent == nullptr) {
			this->root = b;
		} else if (a == a->parent->rchild) {
			a->parent->rchild = b;
		} else {
			a->parent->lchild = b;
		}
		b->rchild = a;
		a->parent = b;
	}

	void rotateLeft(RidePtr a) {   //rotation function
		RidePtr b = a->rchild;
		a->rchild = b->lchild;
		if (b->lchild != TNULL) {
			b->lchild->parent = a;
		}
		b->parent = a->parent;
		if (a->parent == nullptr) {
			this->root = b;
		} else if (a == a->parent->lchild) {
			a->parent->lchild = b;
		} else {
			a->parent->rchild = b;
		}
		b->lchild = a;
		a->parent = b;
	}

    RidePtr createRide(int rideNumber, int rideCost, int rideDuration){
        RidePtr ride = new Ride;
        ride->parent = nullptr;
        ride->rideNumber = rideNumber;
        ride->rideCost = rideCost;
        ride->rideDuration = rideDuration;
        ride->lchild = TNULL;
        ride->rchild = TNULL;
        ride->clr = 1;
        return ride;
    }

	void insert(RidePtr ride) {  //Insert function to insert elements into rbt
        ride->parent = nullptr;
		ride->lchild = TNULL;
		ride->rchild = TNULL;
		ride->clr = 1;
		RidePtr b = nullptr;
		RidePtr a = this->root;

		while (a != TNULL) {
			b = a;
			if (ride->rideNumber < a->rideNumber) {
				a = a->lchild;
			} else {
				a = a->rchild;
			}
		}

		ride->parent = b;
		if (b == nullptr) {
			root = ride;
		} else if (ride->rideNumber < b->rideNumber) {
			b->lchild = ride;
		} else {
			b->rchild = ride;
		}

		if (ride->parent == nullptr){
			ride->clr = 0;
			return;
		}

		if (ride->parent->parent == nullptr) {
			return;
		}

		updateInsert(ride);
	}
	void rideDelete(int rideNumber) {
		rideDeleteHelper(this->root, rideNumber);
	}
	
	RidePtr getroot(){
		return this->root;
	}
};

//createRide function to create a new ride
RidePtr createRide(int rideNumber, int rideCost, int rideDuration){
    RidePtr r = new Ride();
    r->rideCost = rideCost;
    r->rideNumber = rideNumber;
    r->rideDuration = rideDuration;
    return r;
}
//printRide function to print the ride details in specified manner
void printRide(RidePtr r){
    cout<<"("<<r->rideNumber<<","<<r->rideCost<<","<<r->rideDuration<<")";
}

//Insert function to insert ride details to the minheap and rbt tree
void Insert(int rideNumber, int rideCost, int rideDuration,  MinHeap *h, RBTree *rbt){
    RidePtr temp = rbt->search(rideNumber);
	if(temp!=NULL && temp->rideNumber == rideNumber){
		cout<<"Duplicate RideNumber";
		exit(1);
	}	
	RidePtr r = createRide(rideNumber,rideCost,rideDuration);
    h->insertKey(r);
    rbt->insert(r);
}

//Print funtion to search the rbt to fetch the ride details of specified rideNumber
void Print(int rideNumber, RBTree *rbt){
    RidePtr r = rbt->search(rideNumber);
    if (r!=NULL && r->rideNumber == rideNumber){
        printRide(r);
        cout<<endl;
    }
	else{
	cout<<"(0,0,0)"<<endl;
	}
}

//Print funtion to search the rbt to fetch the ride details from rideNumber1 to rideNumber2
void Print(int rideNumber1, int rideNumber2, RBTree *rbt){
    string res = "";
    for(int x = rideNumber1; x<= rideNumber2;x++){
        RidePtr r = rbt->search(x);
        if (r!=NULL && r->rideNumber == x){
            res += "("+to_string(r->rideNumber)+","+to_string(r->rideCost)+","+to_string(r->rideDuration)+"),";
        }
    }
    if(res.length()==0){
        cout<<"(0,0,0)"<<endl;
    }else{
        cout<<res.substr(0,res.length()-1)<<endl;
    }
}

//GetNextRide function to get the minptr details which holds the ride with the lowest cost
//at the heap
void GetNextRide(MinHeap *h, RBTree *rbt){
    RidePtr r = h->extractMin();
    if(r==NULL){
        cout<<"No active ride requests"<<endl;
    }else{
		printRide(r);
		cout<<endl;
        rbt->rideDelete(r->rideNumber);
    }
}

//Cancel ride funstion to cancel a given ride
void cancelRide(int rideNumber, MinHeap *h, RBTree *rbt ){
    RidePtr r = rbt->search(rideNumber);
    if(r!=NULL && r->rideNumber == rideNumber){
        rbt->rideDelete(rideNumber);
        h->delKey(r->heapIndex);
    }
}

//Deep copy ride details to perform any updates to the existing rides
RidePtr deepCopyRide(RidePtr ride){
	RidePtr r = new Ride();
	r-> rideNumber = ride-> rideNumber;
	r-> rideCost = ride-> rideCost;
	r-> rideDuration = ride-> rideDuration;
	return r;
}

// Update Trip function
void updateTrip(int rideNumber, int new_tripDuration, MinHeap *h, RBTree *rbt){
    RidePtr r = rbt->search(rideNumber);
    if(r!=NULL){
        int old_tripDuration = r->rideDuration;
        if(new_tripDuration <= old_tripDuration){
		r->rideDuration = new_tripDuration;	
		}
		else if(new_tripDuration > old_tripDuration){
            if(new_tripDuration > 2*old_tripDuration){
                cancelRide(rideNumber,h,rbt);
            }else{
                int new_rideCost = 10 + r->rideCost;
				r = deepCopyRide(r);
				cancelRide(rideNumber,h,rbt);
				r->rideCost=new_rideCost;
				r->rideDuration = new_tripDuration;
                h->insertKey(r);
                rbt->insert(r);
            }
        }
    }

}
 int main(int argccount, char* argv[]) {
 	MinHeap *h = new MinHeap(); //Create a minheap
    RBTree *rbt = new RBTree(); //Create a rbt tree
	
     if (argccount < 2) {
         cout << "Usage: " << argv[0] << " file_name.txt" << endl; //Read arguments from stdin
         return 1;
     }

     ifstream inp_file(argv[1]);
     if (!inp_file.is_open()) {
         cout << "Unable to open input file: " << argv[1] <<" Please include extension"<< endl;
         return 1;
     }

     ofstream out_file("output_file.txt"); // Create output file to write the program output

     cout.rdbuf(out_file.rdbuf());

     string line;
     while (getline(inp_file, line)) {
         // Read each comand line from the input file and parse the comand line in arguments

         string comand;
         string arg1, arg2;
        
         size_t pos = line.find_first_of("(");
         if (pos != string::npos) {
             comand = line.substr(0, pos);
             line = line.substr(pos + 1);
         } else {
             comand = line;
             line.clear();
         }
         pos = line.find_first_of(" ,");
         if (pos != string::npos) {
             arg1 = line.substr(0, pos);
             line = line.substr(pos + 1);
         } else {
             arg1 = line;
             line.clear();
         }
         pos = line.find_first_of(" ,");
         if (pos != string::npos) {
             arg2 = line.substr(0, pos);
             line = line.substr(pos + 1);
         } else {
             arg2 = line;
             line.clear();
         }

         // Call the necessary functions based on the input comand

         if (comand == "Insert") {
             int rideNumber = stoi(arg1);
             float rideCost = stof(arg2);
             int tripDuration = stoi(line);
             Insert(rideNumber, rideCost, tripDuration,h,rbt);
         } else if (comand == "Print") {
             int rideNumber = stoi(arg1);
             if (arg2.empty()) {
                Print(rideNumber,rbt);
             } else {
                int rideNumber2 = stoi(arg2);
                Print(rideNumber, rideNumber2,rbt);
             }
         } else if (comand == "UpdateTrip") {
            int rideNumber = stoi(arg1);
            int newTripDuration = stoi(arg2);
            updateTrip(rideNumber, newTripDuration,h,rbt);
         } else if (comand == "GetNextRide") {
            GetNextRide(h,rbt);
         } else if (comand == "CancelRide") {
            int rideNumber = stoi(arg1);
            cancelRide(rideNumber,h,rbt);
         } else {
            cout << "Invalid comand encountered: " <<endl;
         }

    }
    inp_file.close();
    out_file.close();
 }