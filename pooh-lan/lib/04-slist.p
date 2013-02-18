include 'slist.inc'

r = make_slist()

r . insert( ~pos r . head() ~data 1 )
r . insert( ~pos r . nth( ~num 1 ) ~data 2 )
r . insert( ~pos r . nth( ~num 2 ) ~data 3 )

println( ~msg 'the list' )

print_list( ~list r )

println( ~msg 'remove the second element' )

r . remove( ~pos r . nth( ~num 2 ) )

print_list( ~list r )

# ** iterate over the list and print the elements **

sub print_list( list )
    
  pos :=  list . nth( ~num 1 )
  while defined( ~arg pos )
     println( ~msg list . data( ~pos pos ) )
     pos := list . next( ~pos pos )
  end
end



