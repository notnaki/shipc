struct Person {
    name: string;
    age: int;
}

impl Person {

    public fn getName(self:&Person) string {
        return self.name;
    }

    fn getAge(self:&Person) int {
        return self.age;
    }

    public fn printAge(self:&Person) void {
        printf(f"Age: {self.getAge()}\n");
    }
}


fn main() void {
    let p:&Person = new Person {"John",  42 };
    p.printAge();
    printf(f"Name: {p.getName()}\n");

}
