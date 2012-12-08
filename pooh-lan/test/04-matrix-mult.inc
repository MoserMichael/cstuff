
sub matrix_scale( factor, matrix )
  
  numrows = size( ~arg matrix )
  numcols = size( ~arg matrix[ 1 ] )


  for i range( ~from 1 ~to numrows )
    for j range( ~from 1 ~to numcols )
       matrix[i][j] = factor * matrix[ i ] [ j ]
    end
  end
end

sub matrix_mult( leftm, rightm )
  
  numleftrows = size( ~arg leftm )
  numleftcols = size( ~arg leftm[ 1 ] )

  numrightrows = size( ~arg rightm )
  numrightcols = size( ~arg rightm[ 1 ] )

  if numleftcols != numrightrows 
    return []
  end

  c = []

  for left_row range( ~from 1 ~to numleftrows )
    for right_col range( ~from 1 ~to numrightcols )

       s = 0
       for n range( ~from 1 ~to numleftcols )
         s = s + leftm[ left_row ] [ n ] * rightm[ n ] [ right_col ]
       end
       c[ left_row ] [ right_col ] = s 
    end
  end
  return c
end


sub print_matrix( matrix )
  
   numrows = size( ~arg matrix )
   numcols = size( ~arg matrix[ 1 ] )

   for i range( ~from 1 ~to numrows )
     for j range( ~from 1 ~to numcols )
        print( ~msg matrix[i][j] .. ' ' )
     end
     println( ~msg ' ') 
   end
end


sub make_upper_triangle( rows, cols )
   c = []
   for i range( ~from 1 ~to rows )
     for j range( ~from 1 ~to cols )
       if i <= j
         c[ i ] [ j ] = 1
       else 
         c[ i ] [ j ] = 0
       end
     end
   end
   return c
end

sub make_identity_matrix( rows, cols )
   c = []
   for i range( ~from 1 ~to rows )
     for j range( ~from 1 ~to cols )
       if i == j 
         c[ i ] [ j ] = 1
       else 
         c[ i ] [ j ] = 0
       end
     end
   end
   return c
end
