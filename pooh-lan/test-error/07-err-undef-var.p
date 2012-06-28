

sub foo()

   println( ~msg undefined_local )

   println( ~msg no_such_global )


   return sub ()
        println( ~msg outer.no_capture )
     end
end
