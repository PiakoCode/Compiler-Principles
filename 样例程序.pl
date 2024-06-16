const m=7, n=85;
var x,y,z,q,r,t<10>;

procedure multiply;
    var a,b;
    begin
        a:=x; b:=y; z:=0;
        for r:=5 downto r>2 do
        begin
            z:=z+1000;
            write(z);
        end;

        if z > 400 then z:=0;

    end;


procedure test;
    var a,b,c,e,d;
    begin
        
        a:=2; b:=3;
        while a>3 do
        begin
            c:=a;
            while c>0 do
            begin
                c:=c-1;
                b:=b+1;
            end;
            a:=a+1;
        end;
        write(a);
        write(b);
        write(c);
    end;



begin
    call test;
    q:=m;
    q := q+1;
    x:=m; y:=n;
    call multiply;
    write(t[1]);
    t[1]:=1000;
    write(t[1]);
    q-=q;
    q++;
    write(q);
    q--;
    ++q;
    --q;
    write(q);
    write(z);
end.


