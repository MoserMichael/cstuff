# test linked list data structure - slist.inc
include 'slist.inc'

r := make_slist()

r . insert( ~pos r . head() ~data 1 )
r . insert( ~pos r . nth( ~num 1 ) ~data 2 )
r . insert( ~pos r . nth( ~num 2 ) ~data 3 )
r . insert( ~pos r . nth( ~num 3 ) ~data 4 )
r . insert( ~pos r . nth( ~num 4 ) ~data 5 )
r . insert( ~pos r . nth( ~num 5 ) ~data 6 )

println( ~msg 'the list' )

print_list( ~list r )


println( ~msg 'mapcopy - square all elements and return them in new list' ) 
square := r . mapcopy( ~func sub( arg ) 
                 return arg * arg 
               end )
print_list( ~list square )               


println( ~msg 'mapreplace - square all elements and replace original list with new values returned by argument func' )
r . mapreplace(  ~func sub( arg )
                 return arg * arg
                end )
print_list( ~list r )

println( ~msg 'foldfirst2last - get sum of all numbers in the list' )

sum = r . foldfirst2last( ~func sub( a, b )
                        return a + b
                      end
                    ~initval 0 )
println( ~msg 'sum of elements is [ sum ]' )

println( ~msg 'filtercopy - create new list where all elements are even' )

square := r . filtercopy( ~func sub( arg )
                            return arg % 2 == 0
                          end )
print_list( ~list square )



println( ~msg 'remove the second element' )

r . remove( ~pos r . nth( ~num 1 ) )

print_list( ~list r )


println( ~msg 'test range iterator / for loop - iterate over all elements' )

for n   r.range()
  println( ~msg n )
end

println( ~msg 'test range iterator / for loop - iterate from second to fourth' )

for n   r.range( ~from 2 ~to 4 )
  println( ~msg n )
end

println( ~msg 'test range iterator / for loop - iterate from second to last' )

for n   r.range( ~from 2 )
  println( ~msg n )
end


# ** iterate over the list and print the elements **

sub print_list( list )
    
  pos :=  list . nth( ~num 1 )
  while defined( ~arg pos )
     println( ~msg list . data( ~pos pos ) )
     pos := list . next( ~pos pos )
  end
end






