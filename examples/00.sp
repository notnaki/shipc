struct Hey{
    m_int: int;
}
struct Hey1{
    m_hey: Hey;
}
struct Hey2{
    m_hey1: Hey1;
}

#struct Next {
#    m_int : int;
#    next : Next;
#}

fn main() int {

    
    let m_hey: Hey = new Hey{11};
    let m_hey1: Hey1 = new Hey1{m_hey};
    let m_hey2: Hey2 = new  Hey2{m_hey1};
    printf("Hey.int:%i", m_hey.m_int);
    printf("Hey1.Hey.int:%i", m_hey1.m_hey.m_int);
    printf("Hey2.Hey1.Hey.int:%i", m_hey2.m_hey1.m_hey.m_int);

    let x: Hey =m_hey2.m_hey1.m_hey;
    let f:Hey = m_hey2.m_hey1;
    let c:Hey = f.m_hey;
    let n:Hey = c;

    printf("|x: %i|", x.m_int);
    printf("|n: %i|", n.m_int);

    #let m_next: Next = new Next{10};
    #let m_next2:Next = new Next{11, m_next};

    #printf("m_next.int: %i", m_next.m_int);
    #printf("m_next2.int: %i  |  m_next2.m_next.int: %i", m_next2.m_int, m_next2.next.m_int);

    return 0;
}


