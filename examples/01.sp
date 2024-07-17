struct Person {
    Name: string;
    Age: int;
    Height: int;
}

fn main() void {
    
    for(let i:int=0; i < 10; i = i+1;){
        let p:&Person = new Person{f"person_{i}", i, i*100};
        printf(f"Person | Name: {p.Name} , Age: {p.Age} , Height: {p.Height} \n");
    }
}