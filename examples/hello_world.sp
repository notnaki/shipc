struct Guy {
    name: string;
    age: int;
}

fn main() void {
    printf("Hello, World!\n");
    let x:[3]&Guy = [3]&Guy{};

    x[0] = new Guy{"John", 25};
    let g:&Guy = x[0];

    printf("Name: %s, Age: %d", g.name, g.age);

}
