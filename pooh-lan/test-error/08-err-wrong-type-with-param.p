
sub foo1(arg)
  arg[1] = 1

  b = arg * 1

end

sub foo2(arg)
  arg[1] = 1

  arg = 3
end

sub foo3(arg)

  c = arg[2]
  
  arg = 3
end

sub foo4(arg)
   arg = 3

   arg{3} = 5
end
