

class Creator{
    public:
        //type is base class pointer
        //create type will be selected polymorphically
        // at run time
        virtual Object* create(ObjectType) = 0;

};

//id would be an enum type with appropriate identifier names
Object* Creator::create(ObjectType tag){
    if (id == obj1){ return new Obj_1; }
    if (id == obj2){ return new Obj_2; }
    //could have more objects here
    else {return 0;}//error - null object
}

Object* Obj_1::create(ObjectType tag){
    if (id == obj1){ return new Obj_2; }
    //override behaviour to change product
    if (id == obj2){ return new Obj_1; }

    //other tags for different behaviour
    else {return nullptr;}//error - null object
}