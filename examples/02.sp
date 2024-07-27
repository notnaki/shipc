struct Person {
    Name: string;
    Age: int;
    Height: int;
}

fn main() void {
    let p:&Person = new Person{"John"};
    printf(f"{p.Name}\n");
    p.Name = "Jack";
    printf(f"{p.Name}\n");


    let i:[2]int = [2]int{1,2};
    printf(f"{i[0]}\n");
    i[0] = 10;
    printf(f"{i[0]}\n");
}
