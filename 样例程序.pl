const m=7, n=85;
var x,y,z,q,r;

procedure multiply;
    var a,b;
    begin
        a:=x; b:=y; z:=0;
        var r;
        for(r:=0; r<10; r++);
        begin
            z:=z+1;
        end;
    end;
begin
    
    q:=m;
    q := q+1;
    x:=m; y:=n;
    call multiply;
    write(x);
    q-=q;
    q++;
    write(q);
    q--;
    ++q;
    --q;
    write(q);
    write(z);
end.


