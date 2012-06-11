
# board is a global variable.

board = []


# function modifies the global varial board
# now everywhere that the global variable is changed from within the function, it has to be prefixed with global . 
# very easy to read resulting code, also practice of requiring to type more shoud discourage practice of modifying globals from functions.

sub fillboard()
  for i range( ~from 1 ~to 10)
     global.board[ i ] = []
     for j range( ~from 1 ~to 10)
        if i == 1
          global.board[i][j] = 1
        elseif i == 10
          global.board[i][j] = -1
        else
          global.board[i][j] = 0
        end
      end
   end
end 


sub printboard()
  for i range( ~from 1 ~to 10 )
    println( ~msg ' ' )
    for j range( ~from 1 ~to 10) 
      if global.board[i][j] == 1
        print( ~msg ' * ' )
      elsif global.board[i][j] == -1 
        print( ~msg ' # ' )
      else
        print( ~msg '   ' )
      end
      println( ~msg ' ' )
    end
  end
end

fillboard()
printboard()
