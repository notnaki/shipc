struct Person {
    name: string;
    age: int;
}

impl Person {

    private fn getName(self:&Person) string {
        return self.name;
    }

    private fn getAge(self:&Person) int {
        return self.age;
    }

    fn printAge(self:&Person) void {
        printf(f"Age: {self.getAge()}\n");
    }
}



fn main() void {
    let p:&Person = new Person {"John",  42 };
    p.getAge();
    p.printAge();

}
