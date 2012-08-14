
sub make_point( x, y )

    return { 'x_pos' : x,
             
             'y_pos' : y,
             
             'move'  : sub ( dx, dy ) 
                            this . x_pos = this . x_pos + dx
                            this . y_pos = this . y_pos + dy

                            return this
                       end,

             'print' : sub ( )
#                           println( ~msg 'point x-pos: [ this . x_pos ]  y-pos: [ this . y_pos ] ' )
                            println( ~msg 'point x-pos: ' .. this . x_pos .. '  y-pos: ' ..  this . y_pos  )
                       end,

             'get_x' : sub ( )
                            return this . x_pos
                       end,

             'get_y' : sub ( )
                            return this . y_pos  
                       end
            }
end            


pt = make_point( ~x 10 ~y 10 )

pt . print ()

pt . move ( ~dx 1 ~dy -1 )

pt . print ()

println( ~msg 'point-accessors x = [ pt . get_x () ]  y = [ pt . get_y() ]' )

             
