

# make a heap - the top is always the smallest key. 
sub make_min_heap()

    return {
	      'values' : [],

	      'push'   : sub( key, data )

			    push( ~array this.values ~top [ key, data ] )

			    n = size( ~arg this.values )
			    while n > 1
				parent = n / 2
				if this.values[ parent ] [ 1 ] > this.values[ n ] [ 1 ] 
				    tmp = this.values[ parent ]
				    this.values[ parent ] = this.values[ n ]
				    this.values[ n ] = tmp
				else
				    break
				end
                                n = parent
			    end
			 end,

              'empty'  : sub() 
                            return size( ~arg this.values ) == 0
                         end,
	      
	      'top'    : sub()
			    if size( ~arg this.values ) == 0
			      return []
			    end
			    return this.values[ 1 ] 
		         end,

	      'pop'    : sub()
			     sz = size( ~arg this.values )
			     if sz == 0
				return []
			     end

			     res = this.values[ 1 ]
			     this.values[ 1 ] = this.values[ sz ]
			     pop( ~array this.values )
			     
			  
			     pos = 1
			     loop
				    rightn = pos * 2
				    leftn = pos * 2 + 1

				    if rightn >= sz
				      break
				    end

				    if leftn >= sz
					if this.values[ pos ][ 1 ] < this.values[ rightn ] [ 1 ]
					   break
					end
					nextn = rightn
				    else

					if this.values[ pos ][ 1 ] < this.values[ rightn ][ 1 ] &&
					    this.values[ pos ][ 1 ] < this.values[ leftn  ][ 1 ]
					    break
					end

				
				        if this.values[ rightn ][ 1 ] < this.values[ leftn ][ 1 ] 
					  nextn = rightn
	    			        else
					  nextn = leftn
				        end
				   end			

				   tmp = this.values[ nextn ]
				   this.values[ nextn ] = this.values[ pos ]
				   this.values[ pos ] = tmp

				   pos = nextn
			     end

			     return res
			 end

	   }
end
